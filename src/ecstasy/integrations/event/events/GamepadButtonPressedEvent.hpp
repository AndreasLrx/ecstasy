///
/// @file GamepadButtonPressedEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-18
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADBUTTONPRESSEDEVENT_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADBUTTONPRESSEDEVENT_HPP_

#include "GamepadButtonEvent.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing a gamepad button pressed.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-17)
    ///
    struct GamepadButtonPressedEvent : public GamepadButtonEvent {
        ///
        /// @brief Construct a new GamepadButtonPressedEvent object.
        ///
        /// @param[in] pId Targeted gamepad id.
        /// @param[in] pButton Targeted button.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr GamepadButtonPressedEvent(size_t pId, Gamepad::Button pButton) noexcept
            : GamepadButtonEvent(pId, pButton, true)
        {
        }
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADBUTTONPRESSEDEVENT_HPP_ */
