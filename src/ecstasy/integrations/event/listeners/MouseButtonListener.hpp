///
/// @file MouseButtonListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-06
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONLISTENER_HPP_
#define ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONLISTENER_HPP_

#include "EventListeners.hpp"

namespace ecstasy::integration::event
{
    struct MouseButtonEvent;

    /// @brief @ref MouseButtonEvent listener.
    using MouseButtonListener = EventListener<MouseButtonEvent>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONLISTENER_HPP_ */
