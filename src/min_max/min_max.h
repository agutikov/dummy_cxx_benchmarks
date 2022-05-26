#pragma once

#include <concepts>
#include <vector>
#include <algorithm>
#include <limits>
#include <ranges>


// Min and Max implementations:
// https://www.geeksforgeeks.org/compute-the-minimum-or-maximum-max-of-two-integers-without-branching/

// https://stackoverflow.com/questions/55109204/is-there-a-branchless-method-to-quickly-find-the-min-max-of-two-double-precision


template<typename T>
struct Std
{
    static T min(T x, T y)
    {
        return std::min(x, y);
    }

    static T max(T x, T y)
    {
        return std::max(x, y);
    }
};

template<typename T>
struct StdMinMax
{
    static T min(T x, T y)
    {
        return std::minmax(x, y).first;
    }

    static T max(T x, T y)
    {
        return std::minmax(x, y).second;
    }
};

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

template<std::floating_point T>
struct Abs<T>
{
    static T abs(T x, T y)
    {
        return std::abs(x - y);
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

    template<std::ranges::range R>
    void consume(R&& range)
    {
        for (T v : range) {
            consume(v);
        }
    }
};
