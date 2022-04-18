//
// Created by wangchunye.
//
#include <benchmark/benchmark.h>
#include "fec_test.h"

FecTest ft;

static void DoSetup(const benchmark::State& state) { ft.Init(); }

static void DoTeardown(const benchmark::State& state) {}

void BM_Encode(benchmark::State& state) {
  for (auto _ : state) {
    //待测试的代码
    ft.Encode();
  }
}

void BM_Decode(benchmark::State& state) {
  for (auto _ : state) {
    ft.Decode();
  }
}

BENCHMARK(BM_Encode)->Setup(DoSetup)->Teardown(DoTeardown);
BENCHMARK(BM_Decode)->Setup(DoSetup)->Teardown(DoTeardown);
BENCHMARK_MAIN();
