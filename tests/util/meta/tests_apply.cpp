#include <gtest/gtest.h>
#include "util/meta/apply.hpp"

TEST(apply, identity)
{
    using got = util::meta::apply_t<std::type_identity_t, int, float, double, std::string>;
    using expected = util::meta::Traits<int, float, double, std::string>;
    bool res = std::is_same_v<got, expected>;

    GTEST_ASSERT_TRUE(res);
}

TEST(apply, remove_reference)
{
    using got = util::meta::apply_t<std::remove_reference_t, int &, float, double &, std::string>;
    using expected = util::meta::Traits<int, float, double, std::string>;
    bool res = std::is_same_v<got, expected>;

    GTEST_ASSERT_TRUE(res);
}