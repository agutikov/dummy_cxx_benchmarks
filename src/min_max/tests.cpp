
#include <algorithm>
#include <limits>
#include <utility>

#include "gtest/gtest.h"

#include "generate.h"
#include "min_max.h"


using namespace testing;


TEST(SubsShift, int64_min_OK)
{
    int64_t x = std::numeric_limits<int64_t>::min();
    int64_t y = 0;

    EXPECT_EQ(SubsShift<int64_t>::max(x, y), y);
    EXPECT_EQ(SubsShift<int64_t>::min(x, y), x);
}

TEST(SubsShift, int64_min_FAIL)
{
    int64_t x = std::numeric_limits<int64_t>::min();
    int64_t y = 1;

    EXPECT_EQ(SubsShift<int64_t>::max(x, y), y);
    EXPECT_EQ(SubsShift<int64_t>::min(x, y), x);
}

TEST(SubsShift, int64_max_OK)
{
    int64_t x = std::numeric_limits<int64_t>::max();
    int64_t y = 0;

    EXPECT_EQ(SubsShift<int64_t>::max(x, y), x);
    EXPECT_EQ(SubsShift<int64_t>::min(x, y), y);
}

TEST(SubsShift, int64_max_FAIL)
{
    int64_t x = std::numeric_limits<int64_t>::max();
    int64_t y = -1;

    EXPECT_EQ(SubsShift<int64_t>::max(x, y), x);
    EXPECT_EQ(SubsShift<int64_t>::min(x, y), y);
}





template<typename T, template<typename> class MinMax, std::ranges::range R>
Stats<T> GetStats(R&& range)
{
    StatsConsumer<T, MinMax> consumer;
    consumer.consume(std::forward<R>(range));
    return consumer.stats;
}


const size_t vector_size = 1'000'000;


TEST(MinMax, random_double)
{
    using value_type = double;

    auto values = Random<value_type>::generate(1);

    ASSERT_EQ(values.size(), 1);

    auto ternary = GetStats<value_type, Ternary>(values);
    auto std_stats = GetStats<value_type, Std>(values);
    auto std_minmax = GetStats<value_type, StdMinMax>(values);
    auto abs = GetStats<value_type, Abs>(values);

    EXPECT_EQ(ternary.max, ternary.min);
    EXPECT_EQ(std_stats.max, std_stats.min);
    EXPECT_EQ(std_minmax.max, std_minmax.min);
    EXPECT_EQ(abs.max, abs.min);

    values = Random<value_type>::generate(vector_size);
    auto v = values;

    ASSERT_EQ(values.size(), vector_size);

    ternary = GetStats<value_type, Ternary>(values);
    std_stats = GetStats<value_type, Std>(values);
    std_minmax = GetStats<value_type, StdMinMax>(values);
    abs = GetStats<value_type, Abs>(values);

    ASSERT_TRUE(std::equal(v.begin(), v.end(), values.begin()));

    Stats<value_type> empty;
    std::cout << "empty=" << empty << std::endl;
    std::cout << "ternary=" << ternary << std::endl;
    std::cout << "std=" << std_stats << std::endl;
    std::cout << "std_minmax=" << std_minmax << std::endl;
    std::cout << "abs=" << abs << std::endl;

    EXPECT_EQ(ternary, std_stats);
    EXPECT_EQ(ternary, std_minmax);
    EXPECT_EQ(ternary, abs);

    EXPECT_NE(empty, ternary);
    EXPECT_NE(empty, abs);
    EXPECT_NE(empty, std_stats);
    EXPECT_NE(empty, std_minmax);
}


TEST(MinMax, random_int)
{
    using value_type = int64_t;

    auto values = Random<value_type>::generate(1);

    ASSERT_EQ(values.size(), 1);

    auto ternary = GetStats<value_type, Ternary>(values);
    auto std_stats = GetStats<value_type, Std>(values);
    auto std_minmax = GetStats<value_type, StdMinMax>(values);
    auto xor_and = GetStats<value_type, XorAnd>(values);
    auto subs_shift = GetStats<value_type, SubsShift>(values);
    auto abs = GetStats<value_type, Abs>(values);

    std::cout << "ternary =    " << ternary << std::endl;
    std::cout << "std =        " << std_stats << std::endl;
    std::cout << "std_minmax = " << std_minmax << std::endl;
    std::cout << "xor_and =    " << xor_and << std::endl;
    std::cout << "subs_shift = " << subs_shift << std::endl;
    std::cout << "abs =        " << abs << std::endl;

    EXPECT_EQ(ternary.max, ternary.min);
    EXPECT_EQ(std_stats.max, std_stats.min);
    EXPECT_EQ(std_minmax.max, std_minmax.min);
    EXPECT_EQ(xor_and.max, xor_and.min);
    EXPECT_EQ(subs_shift.max, subs_shift.min);
    EXPECT_EQ(abs.max, abs.min);

    values = Random<value_type>::generate(vector_size);
    auto v = values;

    ASSERT_EQ(values.size(), vector_size);

    ternary = GetStats<value_type, Ternary>(values);
    std_stats = GetStats<value_type, Std>(values);
    std_minmax = GetStats<value_type, StdMinMax>(values);
    abs = GetStats<value_type, Abs>(values);
    xor_and = GetStats<value_type, XorAnd>(values);
    subs_shift = GetStats<value_type, SubsShift>(values);

    ASSERT_TRUE(std::equal(v.begin(), v.end(), values.begin()));

    Stats<value_type> empty;
    std::cout << "empty =      " << empty << std::endl;
    std::cout << "ternary =    " << ternary << std::endl;
    std::cout << "std =        " << std_stats << std::endl;
    std::cout << "std_minmax = " << std_minmax << std::endl;
    std::cout << "xor_and =    " << xor_and << std::endl;
    std::cout << "subs_shift = " << subs_shift << std::endl;
    std::cout << "abs =        " << abs << std::endl;

    EXPECT_EQ(ternary, std_stats);
    EXPECT_EQ(ternary, std_minmax);
    EXPECT_EQ(ternary, abs);
    EXPECT_EQ(ternary, xor_and);
    EXPECT_EQ(ternary, subs_shift);

    EXPECT_NE(empty, ternary);
    EXPECT_NE(empty, abs);
    EXPECT_NE(empty, std_stats);
    EXPECT_NE(empty, std_minmax);
    EXPECT_NE(empty, xor_and);
    EXPECT_NE(empty, subs_shift);
}


TEST(MinMax, random_uint)
{
    using value_type = uint64_t;

    auto values = Random<value_type>::generate(1);

    ASSERT_EQ(values.size(), 1);

    auto ternary = GetStats<value_type, Ternary>(values);
    auto std_stats = GetStats<value_type, Std>(values);
    auto std_minmax = GetStats<value_type, StdMinMax>(values);
    auto xor_and = GetStats<value_type, XorAnd>(values);
    auto subs_shift = GetStats<value_type, SubsShift>(values);
    auto abs = GetStats<value_type, Abs>(values);

    EXPECT_EQ(ternary.max, ternary.min);
    EXPECT_EQ(std_stats.max, std_stats.min);
    EXPECT_EQ(std_minmax.max, std_minmax.min);
    EXPECT_EQ(xor_and.max, xor_and.min);
    EXPECT_EQ(subs_shift.max, subs_shift.min);
    EXPECT_EQ(abs.max, abs.min);

    values = Random<value_type>::generate(vector_size);
    auto v = values;

    ASSERT_EQ(values.size(), vector_size);

    ternary = GetStats<value_type, Ternary>(values);
    std_stats = GetStats<value_type, Std>(values);
    std_minmax = GetStats<value_type, StdMinMax>(values);
    abs = GetStats<value_type, Abs>(values);
    xor_and = GetStats<value_type, XorAnd>(values);
    subs_shift = GetStats<value_type, SubsShift>(values);

    ASSERT_TRUE(std::equal(v.begin(), v.end(), values.begin()));

    Stats<value_type> empty;
    std::cout << "empty=" << empty << std::endl;
    std::cout << "ternary=" << ternary << std::endl;
    std::cout << "std=" << std_stats << std::endl;
    std::cout << "std_minmax=" << std_minmax << std::endl;
    std::cout << "xor_and" << xor_and << std::endl;
    std::cout << "subs_shift" << subs_shift << std::endl;
    std::cout << "abs=" << abs << std::endl;

    EXPECT_EQ(ternary, std_stats);
    EXPECT_EQ(ternary, std_minmax);
    EXPECT_EQ(ternary, abs);
    EXPECT_EQ(ternary, xor_and);
    EXPECT_EQ(ternary, subs_shift);

    EXPECT_NE(empty, ternary);
    EXPECT_NE(empty, abs);
    EXPECT_NE(empty, std_stats);
    EXPECT_NE(empty, std_minmax);
    EXPECT_NE(empty, xor_and);
    EXPECT_NE(empty, subs_shift);
}
