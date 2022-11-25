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
    struct Action {
        using Id = size_t;

        Id id;
    };
} // namespace ecstasy::integration::user_action

#endif /* !ECSTASY_INTEGRATIONS_USER_ACTION_ACTION_HPP_ */
