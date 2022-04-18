//
// Created by wangchunye on 2017/1/4.
//
#include "fec.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>

namespace testing {

void dumpMatrix(const std::string& name, const std::vector<row_type>& matrix) {
  std::cout << "===== " << name << " =====" << std::endl;
  for (int i = 0; i < matrix.size(); i++) {
    for (auto b : *matrix[i]) {
      // std::cout << int(b) << " ";
      printf("%5d", int(b));
    }
    std::cout << std::endl;
  }
}
TEST(FEC, Test) {
  // config
  int datashard = 5;
  int parityshard = 3;
  int totalshard = datashard + parityshard;
  FEC fec = FEC::New(20, datashard, parityshard);

  // 创建五条随机数据
  byte arr[] = {0, 0, 0};
  std::vector<row_type> shards(totalshard);
  for (int i = 0; i < datashard; i++) {
    for (int j = 0; j < 3; j++) {
      arr[j] = byte(rand() % 255);
    }
    shards[i] = std::make_shared<std::vector<byte>>(arr, &arr[3]);
  }
  // 为3条冗余数据开辟空间
  for (int i = datashard; i < totalshard; i++) {
    shards[i] = std::make_shared<std::vector<byte>>(3);
  }
  // 打印原始数据
  dumpMatrix("rawdata", shards);

  // 编码
  fec.Encode(shards);
  // 打印编码数据
  dumpMatrix("encoded", shards);

  // 丢弃3,4,5数据并且恢复
  std::vector<row_type> recovered;
  for (int i = 0; i < shards.size(); i++) {
    if (i == 3 || i == 4 || i == 5) {
      continue;
    }

    fecPacket pkt;
    pkt.data = shards[i];
    pkt.seqid = i;
    if (i < 5) {
      pkt.flag = typeData;
    } else {
      pkt.flag = typeFEC;
    }
    recovered = fec.Input(pkt);
    if (recovered.size() > 0) {
      dumpMatrix("remove 3,4,5 and recovered", recovered);
    }
  }

  // 断言数据正确
  for (int i = 0; i < recovered.size(); i++) {
    auto& row = *recovered[i];
    for (int j = 0; j < row.size(); j++) {
      EXPECT_EQ(row[j], (*shards[i + 3])[j]);
    }
  }

  int ret = 0;
  EXPECT_EQ(0, 0);
}
}  // namespace testing
