#include <gtest/gtest.h>
#include "ecstasy/thread/LockableView.hpp"
#include "ecstasy/thread/SharedRecursiveMutex.hpp"

using namespace ecstasy::thread;

/// Debugging helpers, this print types T1 and T2 in the compilation error message
template <typename T1, typename T2>
void check_types()
{
    static_assert(std::is_same_v<T1, T2>, "Types T1 and T2 differs.");
}

class LockableTest : public SharedRecursiveMutex {
  public:
    std::string &getValue()
    {
        return value;
    }

    const std::string &getValue() const
    {
        return value;
    }

    std::string value;
};

TEST(LockableView, class_LockableTest)
{
    // Basic type checks
    static_assert(Lockable<LockableTest>);
    static_assert(std::derived_from<LockableView<LockableTest>, LockableViewBase>);
    check_types<LockableView<LockableTest>::LockableType, LockableTest>();
    check_types<LockableView<const LockableTest>::LockableType, const LockableTest>();

    LockableTest lockable;
    const auto &clockable = lockable;
    // Implicit exclusive lock because not const type
    {
        LockableView view(lockable);

        check_types<decltype(view), LockableView<LockableTest>>();
        check_types<decltype(*view), LockableTest &>();
        check_types<decltype(view->getValue()), std::string &>();

        GTEST_ASSERT_EQ(view->get_lock_count(), 1);
        GTEST_ASSERT_EQ(view->get_owner(), std::this_thread::get_id());
    }
    // Lock released
    GTEST_ASSERT_EQ(lockable.get_lock_count(), 0);
    GTEST_ASSERT_EQ(lockable.get_owner(), std::thread::id());

    // Shared lock because not const type
    {
        LockableView view(clockable);
        LockableView<const LockableTest> explicit_view(lockable);

        check_types<decltype(view), decltype(explicit_view)>();
        check_types<decltype(view), LockableView<const LockableTest>>();
        check_types<decltype(*view), const LockableTest &>();
        check_types<decltype(view->getValue()), const std::string &>();

        GTEST_ASSERT_EQ(view->get_lock_count(), 0);
        GTEST_ASSERT_EQ(view->get_owner(), std::thread::id());
    }
}
