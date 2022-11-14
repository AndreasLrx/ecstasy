///
/// @file MouseWheelScrollEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-05
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEWHEELSCROLLEVENT_HPP_
#define ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEWHEELSCROLLEVENT_HPP_

#include "ecstasy/integrations/event/inputs/Mouse.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing when a mouse wheel is scrolled.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-06)
    ///
    struct MouseWheelScrollEvent {
        /// @brief Wheel offset (positive is up/left and negative is down/right)
        float delta;
        /// @brief target wheel.
        Mouse::Wheel wheel;

        ///
        /// @brief Construct a new MouseWheelScrollEvent object.
        ///
        /// @param[in] pDelta Wheel offset.
        /// @param[in] pWheel Targeted wheel.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr MouseWheelScrollEvent(float pDelta = 0, Mouse::Wheel pWheel = Mouse::Wheel::Horizontal)
            : delta(pDelta), wheel(pWheel)
        {
        }
    };

} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEWHEELSCROLLEVENT_HPP_ */
