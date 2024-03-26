///
/// @file MouseButtonEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-05
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEBUTTONEVENT_HPP_
#define ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEBUTTONEVENT_HPP_

#include "ecstasy/integrations/event/inputs/Mouse.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing when a mouse button is pressed or released.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-06)
    ///
    struct MouseButtonEvent {
        /// @brief target button.
        Mouse::Button button;
        /// @brief Whether it was pressed or released.
        bool pressed;

        ///
        /// @brief Construct a new MouseButtonEvent object.
        ///
        /// @param[in] pButton Targeted button.
        /// @param[in] pPressed Whether the button is pressed or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr MouseButtonEvent(Mouse::Button pButton = Mouse::Button::Count, bool pPressed = true)
            : button(pButton), pressed(pPressed)
        {
        }
    };

} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEBUTTONEVENT_HPP_ */
