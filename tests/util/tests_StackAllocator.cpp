///
/// @file test_StackAllocator.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief @ref StackAllocator tests
/// @version 1.0.0
/// @date 2023-11-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include <gtest/gtest.h>

#include "util/StackAllocator.hpp"

class Base {
  public:
    Base(int &reference) : _reference(reference)
    {
        _reference++;
    }

    ~Base()
    {
        _reference--;
    }

    virtual int getId() const
    {
        return 0;
    }

  private:
    std::reference_wrapper<int> _reference;
};

class A : public Base {
  public:
    A(int &reference) : Base(reference)
    {
    }
    virtual int getId() const override
    {
        return 1;
    }

  private:
    char _reserved[15];
};

class B : public Base {
  public:
    B(int &reference) : Base(reference)
    {
    }
    virtual int getId() const override
    {
        return 2;
    }

  private:
    char _reserved[2];
};

class C : public Base {
  public:
    C(int &reference) : Base(reference)
    {
    }

  private:
    char _reserved[51];
};

TEST(StackAllocator, init)
{
    // Testing with 0 as size makes the compilation fails as expected:
    // util::StackAllocator<0, Base> allocator;
    constexpr size_t size = sizeof(A) + sizeof(B) + sizeof(C);
    int ref_counter = 0;

    {
        util::StackAllocator<size, Base> allocator;
        A &a = allocator.instanciate<A>(ref_counter);
        B &b = allocator.instanciate<B>(ref_counter);
        C &c = allocator.instanciate<C>(ref_counter);

        GTEST_ASSERT_EQ(ref_counter, 3);
        GTEST_ASSERT_EQ(c.getId(), 0);
        GTEST_ASSERT_EQ(a.getId(), 1);
        GTEST_ASSERT_EQ(b.getId(), 2);

        util::StackAllocator<size, Base> movedAllocator(std::move(allocator));

        GTEST_ASSERT_EQ(ref_counter, 3);
        GTEST_ASSERT_TRUE(allocator.getInstances().empty());
        // Fetch the new references (old ones were invalidate and testing them would probably cause a segmentation
        // fault)
        a = dynamic_cast<A &>(*movedAllocator.getInstances()[0]);
        b = dynamic_cast<B &>(*movedAllocator.getInstances()[1]);
        c = dynamic_cast<C &>(*movedAllocator.getInstances()[2]);

        GTEST_ASSERT_EQ(c.getId(), 0);
        GTEST_ASSERT_EQ(a.getId(), 1);
        GTEST_ASSERT_EQ(b.getId(), 2);
    }
    // Ensure destructor is called only once
    GTEST_ASSERT_EQ(ref_counter, 0);
}
