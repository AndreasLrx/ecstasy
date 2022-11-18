///
/// @file MouseWheelScrollListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-06
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEWHEELSCROLLLISTENER_HPP_
#define ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEWHEELSCROLLLISTENER_HPP_

#include "EventListeners.hpp"

namespace ecstasy::integration::event
{
    struct MouseWheelScrollEvent;

    /// @brief @ref MouseWheelScrollEvent listener.
    using MouseWheelScrollListener = EventListener<MouseWheelScrollEvent>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEWHEELSCROLLLISTENER_HPP_ */
