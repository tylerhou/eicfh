#include "benchmark/benchmark.h"

int Noop() {
    return 0;
}

static void BM_Noop(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(Noop());
    }
}
BENCHMARK(BM_Noop);
