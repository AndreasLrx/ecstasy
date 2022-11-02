///
/// @file tests_outer_join.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-02
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include <gtest/gtest.h>

#include "util/meta/outer_join.hpp"

using namespace util::meta;

TEST(left_outer_join, all)
{
    {
        using got = left_outer_join_t<Traits<float, int>, Traits<char, float, double>>;
        using expected = Traits<int>;

        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    {
        using got = left_outer_join_t<Traits<float, int>, Traits<float, int, char, long, double>>;
        using expected = Traits<>;

        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    {
        using got = left_outer_join_t<Traits<float, int>, Traits<>>;
        using expected = Traits<float, int>;

        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    {
        using got = left_outer_join_t<Traits<>, Traits<float, int>>;
        using expected = Traits<>;

        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    {
        using got = left_outer_join_t<Traits<char, short, long>, Traits<float, int, short>>;
        using expected = Traits<char, long>;

        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }
}

TEST(right_outer_join, all)
{
    {
        using got = right_outer_join_t<Traits<char, float, double>, Traits<float, int>>;
        using expected = Traits<int>;

        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    {
        using got = right_outer_join_t<Traits<float, int, char, long, double>, Traits<float, int>>;
        using expected = Traits<>;

        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    {
        using got = right_outer_join_t<Traits<>, Traits<float, int>>;
        using expected = Traits<float, int>;

        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    {
        using got = right_outer_join_t<Traits<float, int>, Traits<>>;
        using expected = Traits<>;

        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    {
        using got = right_outer_join_t<Traits<float, int, short>, Traits<char, short, long>>;
        using expected = Traits<char, long>;

        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }
}