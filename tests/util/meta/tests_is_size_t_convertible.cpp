#include <gtest/gtest.h>
#include "util/meta/is_size_t_convertible.hpp"

enum class MyEnum : std::size_t { A = 42, B = 100 };
enum class AnotherEnum { A = 42, B = 100 };

TEST(is_size_t_convertible, all)
{
    static_assert(util::meta::is_size_t_convertible<size_t>);
    static_assert(util::meta::is_size_t_convertible<MyEnum>);
    static_assert(util::meta::is_size_t_convertible<AnotherEnum>);
}
