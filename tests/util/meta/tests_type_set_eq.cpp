///
/// @file tests_type_set_eq.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-02
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include <gtest/gtest.h>
#include <tuple>

#include "util/meta/type_set_eq.hpp"

using namespace util::meta;

TEST(type_set_eq, equals)
{
    {
        bool got = type_set_eq_v<std::tuple<>, std::tuple<>>;
        GTEST_ASSERT_TRUE(got);
    }

    {
        bool got = type_set_eq_v<std::tuple<int>, std::tuple<int>>;
        GTEST_ASSERT_TRUE(got);
    }

    {
        bool got = type_set_eq_v<std::tuple<int, char>, std::tuple<char, int>>;
        GTEST_ASSERT_TRUE(got);
    }

    {
        bool got = type_set_eq_v<std::tuple<int, float, long, char>, std::tuple<long, float, char, int>>;
        GTEST_ASSERT_TRUE(got);
    }
}

TEST(type_set_eq, not_equals)
{
    {
        bool got = type_set_eq_v<std::tuple<int>, std::tuple<>>;
        GTEST_ASSERT_FALSE(got);
    }

    {
        bool got = type_set_eq_v<std::tuple<>, std::tuple<int>>;
        GTEST_ASSERT_FALSE(got);
    }

    {
        bool got = type_set_eq_v<std::tuple<int>, std::tuple<char>>;
        GTEST_ASSERT_FALSE(got);
    }

    {
        bool got = type_set_eq_v<std::tuple<int, float, char>, std::tuple<char, long, float>>;
        GTEST_ASSERT_FALSE(got);
    }
}