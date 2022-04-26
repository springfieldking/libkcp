//
// Created by wangchunye on 2017/1/4.
//
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdio.h>
#include <random>
#include "fec.h"

namespace testing {

void dumpMatrix(const std::string& name, const std::vector<row_type>& matrix) {
  std::cout << "==================== " << name << " ====================" << std::endl;
  for (int i = 0; i < matrix.size(); i++) {
    if (matrix[i] != nullptr) {
      for (auto b : *matrix[i]) {
        printf("%5u", b);
      }
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

  // 随机初始化
  std::random_device rd;
  std::mt19937 mt(rd());

  // 每条长度在1~10内随机
  std::uniform_int_distribution<byte> dist_1_10(1, 10);
  // 每条数据的字节随机0~255
  std::uniform_int_distribution<byte> dist_0_255(0, 255);
  // 创建五条随机数据
  std::vector<std::vector<byte>> ori_data(datashard);
  for (int i = 0; i < datashard; i++) {
    auto len = dist_1_10(mt);
    for (int j = 0; j < len; j++) {
      ori_data[i].push_back(dist_0_255(mt));
    }
  }

  // 把原始数据构建成shards
  std::vector<row_type> shards(totalshard);
  for (int i = 0; i < datashard; i++) {
    shards[i] = std::make_shared<std::vector<byte>>(ori_data[i]);
  }
  dumpMatrix("rawdata", shards);

  // 编码
  fec.Encode(shards);
  dumpMatrix("encoded", shards);

  // 创建随机丢包序列
  std::vector<bool> rand_loss_seq(totalshard, false);
  for (int i = 0; i < parityshard; i++) rand_loss_seq[i] = true;
  std::shuffle(rand_loss_seq.begin(), rand_loss_seq.end(), mt);

  // 把数据包放入fec协议栈
  std::vector<row_type> recovered;
  for (int i = 0; i < totalshard; i++) {
    // 根据随机丢包序列弃包
    if (rand_loss_seq[i]) continue;

    fecPacket pkt;
    pkt.data = shards[i];
    pkt.seqid = i;
    if (i < 5)
      pkt.flag = typeData;
    else
      pkt.flag = typeFEC;

    // 尝试恢复数据
    fec.Input(pkt, 0, recovered);
    if (recovered.size() > 0) {
      dumpMatrix("recovered", recovered);
      break;
    }
  }

  // 断言数据正确
  int recovered_index = 0;
  for (int i = 0; i < datashard; i++) {
    // 如果是丢包数据,查看数据是否正确
    if (!rand_loss_seq[i]) continue;
    auto ori_data_len = ori_data[i].size();
    for (int len = 0; len < ori_data_len; len++) {
      EXPECT_EQ(ori_data[i][len], (*recovered[recovered_index])[len]);
    }
    std::cout << "data index[" << i << "], len=" << ori_data_len << std::endl;
    recovered_index++;
  }
}
}  // namespace testing
