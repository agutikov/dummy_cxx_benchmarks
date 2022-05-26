#pragma once

#include <concepts>
#include <ostream>
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
    static constexpr size_t shift = sizeof(T) * 8 - 1;

    static T min(T x, T y)
    {
        T d = x - y;
        return y + (d & (d >> shift));
    }

    static T max(T x, T y)
    {
        T d = x - y;
        return x - (d & (d >> shift));
    }
};

template<typename T>
struct Abs
{
    static constexpr size_t shift = sizeof(T) * 8 - 1;

    static T abs(T x, T y)
    {
        T sub = x - y;
        T mask = (sub >> shift);
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

    bool operator==(const Stats& other) const
    {
        return other.min == min && other.max == max;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Stats<T>& s)
{
    os << "{" << ".min=" << s.min << ", .max=" << s.max << "}";
    return os;
}


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
