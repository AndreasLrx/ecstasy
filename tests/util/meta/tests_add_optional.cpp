///
/// @file tests_add_optional.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-02
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include <gtest/gtest.h>

#include "util/meta/add_optional.hpp"

using namespace util::meta;

TEST(add_optional, all)
{
    GTEST_ASSERT_EQ(typeid(add_optional_t<int>), typeid(std::optional<int>));
    GTEST_ASSERT_EQ(typeid(add_optional_t<std::optional<int>>), typeid(std::optional<int>));
    GTEST_ASSERT_EQ(typeid(add_optional_t<int &>), typeid(std::optional<std::reference_wrapper<int>>));
}
