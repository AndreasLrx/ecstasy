#include <gtest/gtest.h>
#include "ecstasy/thread/SharedRecursiveMutex.hpp"

using namespace ecstasy::thread;

TEST(SharedRecursiveMutex, exclusive_lock)
{
    SharedRecursiveMutex mutex;

    // No initialisation because of inheritance issue, to fix in a later commit with the whole thread safe integration
    // GTEST_ASSERT_EQ(mutex.get_lock_count(), 0);
    // GTEST_ASSERT_EQ(mutex.get_owner(), std::thread::id());

    /// Exclusive lock
    mutex.lock();
    GTEST_ASSERT_EQ(mutex.get_lock_count(), 1);
    GTEST_ASSERT_EQ(mutex.get_owner(), std::this_thread::get_id());
    mutex.unlock();
    GTEST_ASSERT_EQ(mutex.get_lock_count(), 0);
    GTEST_ASSERT_EQ(mutex.get_owner(), std::thread::id());
}

TEST(SharedRecursiveMutex, shared_locks)
{
    SharedRecursiveMutex mutex;
    const auto &cmutex = mutex;

    // No initialisation because of inheritance issue, to fix in a later commit with the whole thread safe integration
    // GTEST_ASSERT_EQ(mutex.get_lock_count(), 0);
    // GTEST_ASSERT_EQ(mutex.get_owner(), std::thread::id());
    mutex.lock();
    mutex.unlock();

    /// Shared lock
    mutex.lock_shared();
    GTEST_ASSERT_EQ(mutex.get_lock_count(), 0);
    GTEST_ASSERT_EQ(mutex.get_owner(), std::thread::id());
    mutex.unlock_shared();
    GTEST_ASSERT_EQ(mutex.get_lock_count(), 0);
    GTEST_ASSERT_EQ(mutex.get_owner(), std::thread::id());

    /// Const lock leads to shared lock/unlock
    cmutex.lock();
    GTEST_ASSERT_EQ(mutex.get_lock_count(), 0);
    GTEST_ASSERT_EQ(mutex.get_owner(), std::thread::id());
    cmutex.unlock();
    GTEST_ASSERT_EQ(mutex.get_lock_count(), 0);
    GTEST_ASSERT_EQ(mutex.get_owner(), std::thread::id());
}

TEST(SharedRecursiveMutex, recursive_exclusive_lock)
{
    SharedRecursiveMutex mutex;

    // No initialisation because of inheritance issue, to fix in a later commit with the whole thread safe integration
    // GTEST_ASSERT_EQ(mutex.get_lock_count(), 0);
    // GTEST_ASSERT_EQ(mutex.get_owner(), std::thread::id());

    /// Exclusive recursive locks
    /// Scopes are only for better readability, nothing to do with scope variables
    {
        mutex.lock();
        GTEST_ASSERT_EQ(mutex.get_lock_count(), 1);
        GTEST_ASSERT_EQ(mutex.get_owner(), std::this_thread::get_id());

        {
            mutex.lock();
            GTEST_ASSERT_EQ(mutex.get_lock_count(), 2);
            GTEST_ASSERT_EQ(mutex.get_owner(), std::this_thread::get_id());

            {
                mutex.lock();
                GTEST_ASSERT_EQ(mutex.get_lock_count(), 3);
                GTEST_ASSERT_EQ(mutex.get_owner(), std::this_thread::get_id());

                mutex.unlock();
                GTEST_ASSERT_EQ(mutex.get_lock_count(), 2);
                GTEST_ASSERT_EQ(mutex.get_owner(), std::this_thread::get_id());
            }

            mutex.unlock();
            GTEST_ASSERT_EQ(mutex.get_lock_count(), 1);
            GTEST_ASSERT_EQ(mutex.get_owner(), std::this_thread::get_id());
        }

        mutex.unlock();
        GTEST_ASSERT_EQ(mutex.get_lock_count(), 0);
        GTEST_ASSERT_EQ(mutex.get_owner(), std::thread::id());
    }
}
