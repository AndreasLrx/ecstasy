#include <gtest/gtest.h>
#include "ecstasy/storage/Instances.hpp"

class A {
  public:
    virtual std::string getName()
    {
        return "A";
    }

    virtual ~A() = default;
};

class B : public A {
  public:
    virtual std::string getName() override
    {
        return "B";
    }

    virtual ~B() = default;
};

TEST(Instances, all)
{
    ecstasy::Instances<A> instances;

    /// Contains false
    EXPECT_FALSE(instances.contains<B>());
    EXPECT_FALSE(instances.contains(std::type_index(typeid(B))));

    /// Emplace
    EXPECT_EQ(instances.getInner().size(), 0);
    instances.emplace<B>();
    EXPECT_THROW(instances.emplace<B>(), std::logic_error);
    EXPECT_EQ(instances.getInner().size(), 1);

    /// Contains true
    EXPECT_TRUE(instances.contains<B>());
    EXPECT_TRUE(instances.contains(std::type_index(typeid(B))));

    /// Get
    EXPECT_EQ(instances.get<B>().getName(), "B");
    EXPECT_THROW(instances.get<A>().getName(), std::logic_error);
    instances.emplace<A>();
    EXPECT_EQ(instances.get(std::type_index(typeid(A))).getName(), "A");

    /// Clear
    instances.clear();
    EXPECT_EQ(instances.getInner().size(), 0);
    EXPECT_FALSE(instances.contains<A>());
    EXPECT_FALSE(instances.contains<B>());
}