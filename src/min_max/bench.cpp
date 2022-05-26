#include <chrono>

#include "benchmark/benchmark.h"

#include "pp.h"
#include "generate.h"
#include "min_max.h"



template<typename T, template<typename> class Src, template<typename> class MinMax>
static void BM_MinMax(benchmark::State& state)
{
    size_t size = state.range(0);
    std::vector<T> values = Src<T>::generate(size);

    double elapsed_total = 0.0;
    size_t items_total = 0;

    Stats<T> tmp;

    for (auto _ : state) {
        StatsConsumer<T, MinMax> consumer;

        auto started = std::chrono::high_resolution_clock::now();
        consumer.consume(values);
        auto finished = std::chrono::high_resolution_clock::now();

        benchmark::DoNotOptimize(tmp = consumer.stats);

        double elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(finished - started).count();
        state.SetIterationTime(elapsed);

        elapsed_total += elapsed;
        items_total += values.size();
    }

    state.counters["items/s"] = double(items_total) / elapsed_total;
    state.counters["bytes/s"] = double(items_total) * sizeof(T) / elapsed_total;
    state.counters["ns/item"] = elapsed_total * 1000'000'000 / items_total;
}

/*
 * We have 4 sources, 4 int types, 2 float types, 6 int minmax functions, 4 float minmax functions.
 * To cover all combinations need to call
 *     BENCHMARK_TEMPLATE(BM_MinMax, <TYPE>, <SOURCE>, <MINMAX>)
 * 4*4*6 + 4*2*4 = 128 times.
 * Let's use some Boost preprocessor magic for that.
 */

#define INT_TYPES (uint32_t, int32_t, uint64_t, int64_t)
#define FLOAT_TYPES (float, double)
#define SOURCES (falling_v, growing_v, const_v, random_v)
#define INT_MINMAX (Std, StdMinMax, Ternary, XorAnd, SubsShift, Abs)
#define FLOAT_MINMAX (Std, StdMinMax, Ternary, Abs)


static const int64_t min_vector_size = 100;
static const int64_t max_vector_size = 100'000;
static const int64_t range_multiplier = 10;

#define BM_MIN_MAX(TYPE_ARG, SOURCE_ARG, MINMAX_ARG) \
    BENCHMARK_TEMPLATE(BM_MinMax, TYPE_ARG, SOURCE_ARG, MINMAX_ARG) \
    ->UseManualTime() \
    ->RangeMultiplier(range_multiplier) \
    ->Range(min_vector_size, max_vector_size)


#define CALL_BM_MIN_MAX(r, SEQ) APPLY_SEQ(BM_MIN_MAX, SEQ)

APPLY_PRODUCT(CALL_BM_MIN_MAX, FLOAT_TYPES, SOURCES, FLOAT_MINMAX);
APPLY_PRODUCT(CALL_BM_MIN_MAX, INT_TYPES, SOURCES, INT_MINMAX);


BENCHMARK_MAIN();
