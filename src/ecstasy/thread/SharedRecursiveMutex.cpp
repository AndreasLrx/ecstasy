///
/// @file SharedRecursiveMutex.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-03-30
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "SharedRecursiveMutex.hpp"

namespace ecstasy::thread
{
    void SharedRecursiveMutex::lock(void)
    {
        std::thread::id this_id = std::this_thread::get_id();

        if (_owner == this_id) {
            // recursive locking
            _lock_count++;
        } else {
            // Unlock the shared lock if any
            if (get_shared_lock_count() > 0) {
                _shared_mutex.unlock_shared();
                // Add a count to avoid the unlock_shared() to be called before the unlock()
                // While the thread has exclusive lock, there is no real shared_lock, therefore calling unlock_shared()
                // would cause an error
                ++_shared_locks[this_id];
            }
            // normal locking
            _shared_mutex.lock();
            _owner = this_id;
            _lock_count = 1;
        }
    }

    void SharedRecursiveMutex::lock_shared(void) const
    {
        std::thread::id this_id = std::this_thread::get_id();
        // Increment the shared lock count for this thread
        int &shared_lock_count = ++_shared_locks[this_id];

        // If it is the first shared lock, the count is set to 1 by incrementing the 0 default initialized value
        if (shared_lock_count == 1) {
            if (_owner != this_id)
                _shared_mutex.lock_shared();
            else
                ++shared_lock_count;
        }
    }

    void SharedRecursiveMutex::unlock(void)
    {
        if (_lock_count > 1) {
            // recursive unlocking
            _lock_count--;
        } else {
            // normal unlocking
            _owner = std::thread::id();
            _lock_count = 0;
            _shared_mutex.unlock();
            // Apply the remaining shared locks that were made before/inside the exclusive lock
            if (get_shared_lock_count() > 0) {
                // Remove the count added in lock() and call remaininc lock_shared()
                if (--_shared_locks[std::this_thread::get_id()] > 0)
                    _shared_mutex.lock_shared();
            }
        }
    }

    void SharedRecursiveMutex::unlock_shared(void) const
    {
        int &_shared_lock_count = --_shared_locks.at(std::this_thread::get_id());

        // If it was the last shared lock, unlock the shared mutex
        if (_shared_lock_count == 0)
            _shared_mutex.unlock_shared();
    }

    int SharedRecursiveMutex::get_shared_lock_count(void) const noexcept
    {
        // Better than handling exceptions
        if (has_shared_lock())
            return _shared_locks.at(std::this_thread::get_id());
        else
            return 0;
    }

    bool SharedRecursiveMutex::has_shared_lock(void) const noexcept
    {
        return const_cast<const std::unordered_map<std::thread::id, int> &>(_shared_locks)
                   .find(std::this_thread::get_id())
            != _shared_locks.cend();
    }

} // namespace ecstasy::thread
