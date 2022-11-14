///
/// @file MouseButtonPressedEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-05
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEBUTTONPRESSEDEVENT_HPP_
#define ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEBUTTONPRESSEDEVENT_HPP_

#include "ecstasy/integrations/event/inputs/Mouse.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing when a mouse button is pressed.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-06)
    ///
    struct MouseButtonPressedEvent {
        /// @brief button pressed.
        Mouse::Button button;

        ///
        /// @brief Construct a new MouseButtonPressedEvent object.
        ///
        /// @param[in] pButton button pressed.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr MouseButtonPressedEvent(Mouse::Button pButton = Mouse::Button::Count) : button(pButton)
        {
        }
    };

} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEBUTTONPRESSEDEVENT_HPP_ */
