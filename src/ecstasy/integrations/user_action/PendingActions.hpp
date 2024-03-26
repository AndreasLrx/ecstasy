///
/// @file PendingActions.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-07
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONSQUEUE_HPP_
#define ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONSQUEUE_HPP_

#include <queue>
#include "Action.hpp"
#include "ecstasy/resources/ObjectWrapper.hpp"

namespace ecstasy::integration::user_action
{
    /// @brief Queue of @ref Action.
    using PendingActions = ecstasy::ObjectWrapper<std::queue<Action>>;
} // namespace ecstasy::integration::user_action

#endif /* !ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONSQUEUE_HPP_ */
