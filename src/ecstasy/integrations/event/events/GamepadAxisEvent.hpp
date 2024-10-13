///
/// @file GamepadAxisEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-18
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADAXISEVENT_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADAXISEVENT_HPP_

#include "ecstasy/integrations/event/inputs/Gamepad.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing a gamepad axis value change.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-18)
    ///
    struct GamepadAxisEvent {
        /// @brief Target gamepad id.
        size_t id;
        /// @brief Target axis.
        Gamepad::Axis axis;
        /// @brief New axis value.
        float value;

        ///
        /// @brief Construct a new GamepadAxisEvent object.
        ///
        /// @param[in] pId Target gamepad id.
        /// @param[in] pAxis Target axis.
        /// @param[in] pValue new axis value.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr GamepadAxisEvent(size_t pId, Gamepad::Axis pAxis, float pValue) noexcept
            : id(pId), axis(pAxis), value(pValue)
        {
        }
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADAXISEVENT_HPP_ */
