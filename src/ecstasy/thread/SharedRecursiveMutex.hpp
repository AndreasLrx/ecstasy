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

namespace ecstasy::thread
{
    ///
    /// @brief Wrapper for @ref std::shared_mutex allowing recursive locking by the same thread.
    ///
    /// @note Recursive locking is only allowed if the thread locks the mutex with exclusive access.
    /// @warning Deadlock always appears if a thread locks the mutex as shared and then tries to lock it normally.
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
        /// @note Waits until the mutex is available for exclusive access.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        void lock();

        ///
        /// @brief Lock the mutex with shared access.
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
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-02)
        ///
        void unlock();

        ///
        /// @brief Unlock the mutex locked with shared access.
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
        mutable std::shared_mutex _shared_mutex;
        std::atomic<std::thread::id> _owner;
        int _lock_count;
    };

} // namespace ecstasy::thread

#endif // !ECSTASY_THREAD_SHAREDRECURSIVEMUTEX_HPP_
