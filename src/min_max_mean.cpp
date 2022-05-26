#include <algorithm>
#include <ostream>
#include <vector>
#include <random>
#include <functional>
#include <limits>
#include <numeric>
#include <iostream>
#include <chrono>
#include <concepts>

#include <benchmark/benchmark.h>

#include "pp_product.h"


// Min and Max implementations:
// https://www.geeksforgeeks.org/compute-the-minimum-or-maximum-max-of-two-integers-without-branching/


template<typename T>
struct Ternary
{
    static T min(T rhs, T lhs)
    {
        return (rhs < lhs) ? rhs : lhs;
    }

    static T max(T rhs, T lhs)
    {
        return (rhs > lhs) ? rhs : lhs;
    }
};

template<typename T>
struct XorAnd
{
    static T min(T x, T y)
    {
        return y ^ ((x ^ y) & -(x < y));
    }

    static T max(T x, T y)
    {
        return x ^ ((x ^ y) & -(x < y));
    }
};

template<typename T>
struct SubsShift
{
    static T min(T x, T y)
    {
        return y + ((x - y) & ((x - y) >> (sizeof(T) * 8 - 1)));
    }

    static T max(T x, T y)
    {
        return x - ((x - y) & ((x - y) >> (sizeof(T) * 8 - 1)));
    }
};

template<typename T>
struct Abs
{
    static T abs(T x, T y)
    {
        T sub = x - y;
        T mask = (sub >> (sizeof(T)*8 - 1));
        return (sub ^ mask) - mask;
    }

    static T min(T x, T y)
    {
        T a = abs(x, y);
        return (x + y + a) / 2;
    }

    static T max(T x, T y)
    {
        T a = abs(x, y);
        return (x + y - a) / 2;
    }
};



template<typename T>
struct Stats
{
    T min = std::numeric_limits<T>::max();
    T max = std::numeric_limits<T>::min();
};


template<typename T, template<typename> class MinMax>
struct StatsConsumer
{
    Stats<T> stats;

    void consume(T v)
    {
        stats.min = MinMax<T>::min(stats.min, v);
        stats.max = MinMax<T>::max(stats.max, v);
    }

    void consume(const std::vector<T>& values) // TODO: range
    {
        for (T v : values) {
            consume(v);
        }
    }
};




template<typename T>
struct uniform_dist
{};

template<std::integral T>
struct uniform_dist<T>
{
    using type = std::uniform_int_distribution<T>;
};

template<std::floating_point T>
struct uniform_dist<T>
{
    using type = std::uniform_real_distribution<T>;
};

static std::random_device rand_dev;

template<typename T>
struct random_v
{
    static std::vector<T> generate(size_t size)
    {
        T v_min = std::numeric_limits<T>::min();
        T v_max = std::numeric_limits<T>::max();
        std::default_random_engine e1(rand_dev());
        typename uniform_dist<T>::type dist(v_min, v_max);
        std::vector<T> v(size);
        std::generate(v.begin(), v.end(), [&e1, &dist]() -> T { return dist(e1); });
        return v;
    }
};


template<typename T>
struct growing_v
{
    static std::vector<T> generate(size_t size)
    {
        std::vector<T> v(size);
        T start = 0;
        std::iota(v.begin(), v.end(), start);
        return v;
    }
};

template<typename T>
struct falling_v
{
    static std::vector<T> generate(size_t size)
    {
        std::vector<T> v(size);
        T start = std::numeric_limits<T>::max() / 2;
        std::generate(v.begin(), v.end(), [start]() mutable -> T { return start--; });
        return v;
    }
};

template<typename T>
struct const_v
{
    static std::vector<T> generate(size_t size)
    {
        std::vector<T> v(size);
        std::fill(v.begin(), v.end(), 0);
        return v;
    }
};



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

static const int64_t min_vector_size = 10'000;
static const int64_t max_vector_size = 10'000;
static const int64_t range_multiplier = 10;

#define BM_MIN_MAX(TYPE_ARG, SOURCE_ARG, CONSUMER_ARG) \
    BENCHMARK_TEMPLATE(BM_MinMax, TYPE_ARG, SOURCE_ARG, CONSUMER_ARG) \
    ->UseManualTime() \
    ->RangeMultiplier(range_multiplier) \
    ->Range(min_vector_size, max_vector_size)


#define TYPES (uint32_t, int32_t, uint64_t, int64_t)
#define SOURCES (falling_v, growing_v, const_v, random_v)
#define MINMAX (Ternary, XorAnd, SubsShift, Abs)


#define CALL_BM_MIN_MAX(r, SEQ) APPLY_SEQ(BM_MIN_MAX, SEQ)


APPLY_PRODUCT(CALL_BM_MIN_MAX, TYPES, SOURCES, MINMAX);




BENCHMARK_MAIN();

