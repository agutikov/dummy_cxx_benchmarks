#pragma once

#include <vector>
#include <random>
#include <limits>


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
struct Random
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
struct Growing
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
struct Falling
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
struct Constant
{
    static std::vector<T> generate(size_t size)
    {
        std::vector<T> v(size);
        std::fill(v.begin(), v.end(), 0);
        return v;
    }
};
