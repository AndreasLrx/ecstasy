///
/// @file Action.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_USER_ACTION_ACTION_HPP_
#define ECSTASY_INTEGRATIONS_USER_ACTION_ACTION_HPP_

#include <cstddef>

namespace ecstasy::integration::user_action
{
    ///
    /// @brief Describe an action changed event.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-02)
    ///
    struct Action {
        /// @brief Action identifier type.
        using Id = size_t;

        /// @brief Constant id matching any action ID.
        static constexpr Id All = static_cast<size_t>(-1);

        /// @brief Action identifier.
        Id id;
        /// @brief Sender identifier.
        size_t senderId;
        /// @brief New action value.
        float value;
    };

} // namespace ecstasy::integration::user_action

#endif /* !ECSTASY_INTEGRATIONS_USER_ACTION_ACTION_HPP_ */
