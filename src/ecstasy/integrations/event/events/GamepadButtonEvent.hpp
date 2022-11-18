///
/// @file GamepadButtonEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-18
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADBUTTONEVENT_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADBUTTONEVENT_HPP_

#include "ecstasy/integrations/event/inputs/Gamepad.hpp"

namespace ecstasy::integration::event
{
    struct GamepadButtonEvent {
        /// @brief Target gamepad id.
        size_t id;
        /// @brief Target button.
        Gamepad::Button button;
        /// @brief Whether the button is pressed (down) or not (up).
        bool pressed;

        ///
        /// @brief Construct a new GamepadButtonEvent object.
        ///
        /// @param[in] pId Targeted gamepad id.
        /// @param[in] pButton Targeted button.
        /// @param[in] pPressed Whether the button is pressed or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr GamepadButtonEvent(size_t pId, Gamepad::Button pButton, bool pPressed)
            : id(pId), button(pButton), pressed(pPressed)
        {
        }
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADBUTTONEVENT_HPP_ */
