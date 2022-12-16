#include <gtest/gtest.h>
#include "util/meta/filter.hpp"

TEST(filter, arithmetic)
{
    using got = util::meta::filter_t<std::is_arithmetic, int, float, double, std::string>;
    using expected = util::meta::Traits<int, float, double>;
    bool res = std::is_same_v<got, expected>;

    GTEST_ASSERT_TRUE(res);
}

TEST(filter, is_floating)
{
    using got = util::meta::filter_t<std::is_floating_point, int, float, double, std::string>;
    using expected = util::meta::Traits<float, double>;
    bool res = std::is_same_v<got, expected>;

    GTEST_ASSERT_TRUE(res);
}