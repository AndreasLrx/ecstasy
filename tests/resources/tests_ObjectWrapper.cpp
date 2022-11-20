#include <gtest/gtest.h>
#include "ecstasy/resources/ObjectWrapper.hpp"

TEST(ObjectWrapper, int)
{
    ecstasy::ObjectWrapper<int> wrapper(5);

    GTEST_ASSERT_EQ(wrapper.get(), 5);
    GTEST_ASSERT_EQ(const_cast<const ecstasy::ObjectWrapper<int> &>(wrapper).get(), 5);
}