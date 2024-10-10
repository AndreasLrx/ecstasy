#include <gtest/gtest.h>
#include "util/meta/is_type_bounded_array.hpp"

TEST(is_type_bounded_array, all)
{
    bool res;

    // True tests
    res = util::meta::is_type_bounded_array_v<int[5], int>;
    GTEST_ASSERT_TRUE(res);

    res = util::meta::is_type_bounded_array_v<char[5], char>;
    GTEST_ASSERT_TRUE(res);

    res = util::meta::is_type_bounded_array_v<int[5][2], int[2]>;
    GTEST_ASSERT_TRUE(res);

    // False tests
    res = util::meta::is_type_bounded_array_v<int[5], char>;
    GTEST_ASSERT_FALSE(res);

    res = util::meta::is_type_bounded_array_v<int, int>;
    GTEST_ASSERT_FALSE(res);
}
