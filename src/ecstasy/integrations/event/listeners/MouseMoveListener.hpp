///
/// @file MouseMoveListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-06
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEMOVELISTENER_HPP_
#define ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEMOVELISTENER_HPP_

#include "EventListeners.hpp"

namespace ecstasy::integration::event
{
    struct MouseMoveEvent;

    /// @brief @ref MouseMoveEvent listener.
    using MouseMoveListener = EventListener<MouseMoveEvent>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEMOVELISTENER_HPP_ */
