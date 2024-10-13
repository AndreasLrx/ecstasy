///
/// @file MouseButtonReleasedEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-05
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEBUTTONRELEASEDEVENT_HPP_
#define ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEBUTTONRELEASEDEVENT_HPP_

#include "MouseButtonEvent.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing when a mouse button is released.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-06)
    ///
    struct MouseButtonReleasedEvent : public MouseButtonEvent {
        ///
        /// @brief Construct a new MouseButtonReleasedEvent object.
        ///
        /// @param[in] pButton Targeted button.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr MouseButtonReleasedEvent(Mouse::Button pButton) noexcept : MouseButtonEvent(pButton, false)
        {
        }
    };

} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEBUTTONRELEASEDEVENT_HPP_ */
