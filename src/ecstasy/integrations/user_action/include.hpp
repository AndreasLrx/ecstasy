///
/// @file include.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-03-26
///
/// @copyright Copyright (c) ECSTASY 2022-2024
///
///

#ifndef ECSTASY_INTEGRATIONS_USER_ACTION_HPP_
#define ECSTASY_INTEGRATIONS_USER_ACTION_HPP_

#include "Action.hpp"
#include "ActionBinding.hpp"
#include "ActionBindings.hpp"
#include "ActionListener.hpp"
#include "PendingActions.hpp"
#include "PollActions.hpp"
#include "UserProfile.hpp"
#include "Users.hpp"

///
/// @brief User action integration.
/// Includes user action bindings (link events to actions) and user action listeners.
///
/// @note This depends on the @ref ecstasy::integration::event integration.
/// @note Enabled by @b ECSTASY_INTEGRATIONS_USER_ACTION.
///
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @since 1.0.0 (2024-03-26)
///
namespace ecstasy::integration::user_action
{
}

#endif /* !ECSTASY_INTEGRATIONS_USER_ACTION_HPP_ */
