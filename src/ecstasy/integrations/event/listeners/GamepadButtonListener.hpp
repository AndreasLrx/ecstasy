///
/// @file GamepadButtonListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-18
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_LISTENERS_GAMEPADBUTTONLISTENER_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_LISTENERS_GAMEPADBUTTONLISTENER_HPP_

#include "EventListeners.hpp"

namespace ecstasy::integration::event
{
    struct GamepadButtonEvent;

    /// @brief @ref GamepadButtonEvent listener.
    using GamepadButtonListener = EventListener<GamepadButtonEvent>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_LISTENERS_GAMEPADBUTTONLISTENER_HPP_ */
