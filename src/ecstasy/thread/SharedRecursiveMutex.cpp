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
            // normal locking
            _shared_mutex.lock();
            _owner = this_id;
            _lock_count = 1;
        }
    }

    void SharedRecursiveMutex::lock_shared(void) const
    {
        _shared_mutex.lock_shared();
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
        }
    }

    void SharedRecursiveMutex::unlock_shared(void) const
    {
        _shared_mutex.unlock_shared();
    }

} // namespace ecstasy::thread
