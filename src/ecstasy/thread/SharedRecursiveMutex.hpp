///
/// @file SharedRecursiveMutex.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-03-30
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_THREAD_SHAREDRECURSIVEMUTEX_HPP_
#define ECSTASY_THREAD_SHAREDRECURSIVEMUTEX_HPP_

#include <atomic>
#include <thread>
#include <shared_mutex>
#include <unordered_map>

namespace ecstasy::thread
{
    ///
    /// @brief Wrapper for @ref std::shared_mutex allowing recursive locking by the same thread.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-02)
    ///
    class SharedRecursiveMutex {
      public:
        ///
        /// @brief Construct a new Shared Recursive Mutex.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        SharedRecursiveMutex() = default;

        ///
        /// @brief Destroy the Shared Recursive Mutex.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        ~SharedRecursiveMutex() = default;

        ///
        /// @brief Lock the mutex with exclusive access.
        ///
        /// @note Waits until the mutex is available for exclusive access. (ie no shared or exclusive lock held by @b
        /// other threads)
        /// @note If the lock is already acquired, only increment the lock count.
        /// @note If the mutex has a shared lock held by the current thread, it will be upgraded to an exclusive lock
        /// and downgraded after unlock.
        /// For example : lock_shared(); lock(); unlock(); unlock_shared();
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        void lock();

        ///
        /// @brief Lock the mutex with shared access.
        ///
        /// @note If the shared lock is already acquired, only increment the lock count.
        /// @note If the mutex has an exclusive lock held by the current thread, the shared lock request will be
        /// "stored" and applied after exclusive unlock if unlock_shared was not called before.
        /// For example: lock(); lock_shared(); unlock(); unlock_shared();
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        void lock_shared() const;

        /// @copybrief lock_shared
        void lock() const
        {
            lock_shared();
        }

        ///
        /// @brief Unlock the mutex locked with exclusive access.
        ///
        /// @note Decrement the lock count and unlock the mutex if the new lock count is 0.
        /// @note If the mutex had shared locks held by the current thread before lock (and not unlocked until now),
        /// they will be re enabled.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        void unlock();

        ///
        /// @brief Unlock the mutex locked with shared access.
        ///
        /// @note Decrement the shared lock count and unlock the mutex if the new lock count is 0.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        void unlock_shared() const;

        /// @copybrief unlock_shared
        void unlock() const
        {
            unlock_shared();
        }

        ///
        /// @brief Get the internal mutex.
        ///
        /// @return constexpr const @ref std::shared_mutex& internal mutex.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        constexpr const std::shared_mutex &get_shared_mutex(void) noexcept
        {
            return _shared_mutex;
        }

        ///
        /// @brief Get the number of recursive locks held by the current thread.
        ///
        /// @note 0 means the mutex is not locked by the current thread (or at least not in exclusive).
        /// @warning This value is not thread-safe and should only be used for debugging purposes.
        ///
        /// @return constexpr int number of recursive locks held by the current thread.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        constexpr int get_lock_count(void) const noexcept
        {
            return _lock_count;
        }

        ///
        /// @brief Get the number of recursive shared locks held by the current thread.
        ///
        /// @note 0 means the mutex has no shared lock held by the current thread. The shared lock count is also
        /// incremented by 1 when the mutex is locked exclusively (only once) to keep track of the shared lock count
        /// before/within an exclusive lock to unset them during the exclusive lock and reset them after.
        /// Therefore the real count of shared locks is the return value minus 1 if the owner is not null
        /// (std::thread::id()).
        /// @warning This value is exposed for debugging purposes, you should not rely on it for your locks, use the
        /// lock/unlock methods instead.
        ///
        /// @return constexpr int number of recursive shared locks held by the current thread.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-20)
        ///
        int get_shared_lock_count(void) const noexcept;

        ///
        /// @brief Get the owner of the mutex.
        ///
        /// @note If the mutex is not locked, the owner is @ref std::thread::id().
        /// @note This value is thread safe.
        ///
        /// @return constexpr const std::atomic<std::thread::id>& owner of the mutex.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        constexpr const std::atomic<std::thread::id> &get_owner(void) const noexcept
        {
            return _owner;
        }

      private:
        bool has_shared_lock(void) const noexcept;

        mutable std::shared_mutex _shared_mutex;
        std::atomic<std::thread::id> _owner;
        int _lock_count;
        mutable std::unordered_map<std::thread::id, int> _shared_locks;
    };

} // namespace ecstasy::thread

#endif // !ECSTASY_THREAD_SHAREDRECURSIVEMUTEX_HPP_
