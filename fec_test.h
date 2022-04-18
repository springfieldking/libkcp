#include <sys/time.h>
#include <iostream>
#include "fec.h"

class FecTest {
 public:
  void Init() {
    if (inited) return;
    // 随机生成测试数据
    std::cout << "random data init ..." << std::endl;
    for (int i = 0; i < datashard; i++) {
      for (int j = 0; j < pkg_size; j++) {
        raw_pkgs[i][j] = byte(rand() % 255);
      }
    }

    // init once
    for (int i = 0; i < datashard; i++) {
      data_parity_shards[i] = std::make_shared<std::vector<byte>>(raw_pkgs[i], &raw_pkgs[i][pkg_size]);
    }
    // encode once, for decode
    Encode();
    inited = true;
  }

  void Encode() {
    // 编码
    fec.Encode(data_parity_shards);
  }

  void Decode() {
    for (int i = 0; i < data_parity_shards.size(); i++) {
      if (i == 0) {
        // drop first data
        continue;
      }

      fecPacket pkt;
      pkt.data = data_parity_shards[i];
      pkt.seqid = i;
      if (i < datashard) {
        pkt.flag = typeData;
      } else {
        pkt.flag = typeFEC;
      }
      static thread_local std::vector<row_type> recovered;
      recovered.clear();
      fec.Input(pkt, 0, recovered);
      if (recovered.size() > 0) {
        // std::cout << "input data index:" << i << ", recovered:" << recovered.size() << std::endl;
      }
    }
  }

 private:
  bool inited = false;  // 初始化标记

  const static int datashard = 5;                         // 数据分片数量
  const static int parityshard = 1;                       // 纠错分片数量
  const static int totalshard = datashard + parityshard;  // 总的分片数量
  const static int pkg_size = 512;                        //测试数据包大小

  FEC fec = FEC::New(20, datashard, parityshard);  // fec编码器
  int raw_pkgs[datashard][pkg_size];               //  原始数据包
  std::vector<row_type> data_parity_shards = std::vector<row_type>(totalshard);
};