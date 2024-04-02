#include <gtest/gtest.h>
#include "ecstasy/thread/Lockable.hpp"
#include "ecstasy/thread/SharedRecursiveMutex.hpp"

using namespace ecstasy::thread;

TEST(Lockable, SharedRecursiveMutex)
{
    static_assert(Lockable<SharedRecursiveMutex>);
    static_assert(Lockable<const SharedRecursiveMutex>);
    static_assert(!Lockable<int>);
}
