///
/// @file KeyListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYLISTENER_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYLISTENER_HPP_

#include "EventListeners.hpp"

namespace ecstasy::integration::event
{
    struct KeyEvent;

    /// @brief @ref KeyEvent listener.
    using KeyListener = EventListener<KeyEvent>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYLISTENER_HPP_ */
