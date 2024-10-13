///
/// @file MouseMoveEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-05
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEMOVEEVENT_HPP_
#define ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEMOVEEVENT_HPP_

#include "ecstasy/integrations/event/inputs/Mouse.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing when the mouse move.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-06)
    ///
    struct MouseMoveEvent {
        /// @brief New X mouse position.
        int x;
        /// @brief New Y mouse position.
        int y;

        ///
        /// @brief Construct a new MouseMoveEvent object.
        ///
        /// @param[in] pX Mouse X position.
        /// @param[in] pY Mouse Y position.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr MouseMoveEvent(int pX = 0, int pY = 0) noexcept : x(pX), y(pY)
        {
        }
    };

} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENT_EVENTS_MOUSEMOVEEVENT_HPP_ */
