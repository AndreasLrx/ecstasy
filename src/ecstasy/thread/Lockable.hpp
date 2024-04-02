///
/// @file Queryable.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_THREAD_LOCKABLE_HPP_
#define ECSTASY_THREAD_LOCKABLE_HPP_

#include <concepts>

namespace ecstasy::thread
{
    ///
    /// @brief Defines a type that can be locked.
    ///
    /// @tparam L Evaluated Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (20224-31-03)
    ///
    template <typename L>
    concept Lockable = requires(L &lockable) {
        // clang-format off
        { lockable.lock() } -> std::same_as<void>;
        { lockable.unlock() } -> std::same_as<void>;
        // clang-format on
    };

} // namespace ecstasy::thread

#endif /* !ECSTASY_THREAD_LOCKABLE_HPP_ */
