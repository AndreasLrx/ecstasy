///
/// @file tests_contains.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-02
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include <gtest/gtest.h>

#include "util/meta/contains.hpp"

using namespace util::meta;

TEST(contains, true)
{
    {
        bool got = contains<int, int>;
        GTEST_ASSERT_TRUE(got);
    }

    {
        bool got = contains<int, int, int, int>;
        GTEST_ASSERT_TRUE(got);
    }

    {
        bool got = contains<int, int, float, char>;
        GTEST_ASSERT_TRUE(got);
    }

    {
        bool got = contains<int, float, int, char>;
        GTEST_ASSERT_TRUE(got);
    }

    {
        bool got = contains<int, char, float, int>;
        GTEST_ASSERT_TRUE(got);
    }
}

TEST(contains, false)
{
    {
        bool got = contains<int>;
        GTEST_ASSERT_FALSE(got);
    }

    {
        bool got = contains<int, float>;
        GTEST_ASSERT_FALSE(got);
    }

    {
        bool got = contains<int, float, char, long>;
        GTEST_ASSERT_FALSE(got);
    }
}