///
/// @file TextEnteredListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENTS_LISTENERS_TEXTENTEREDLISTENER_HPP_
#define ECSTASY_INTEGRATIONS_EVENTS_LISTENERS_TEXTENTEREDLISTENER_HPP_

#include "EventListeners.hpp"

namespace ecstasy::integration::event
{
    struct TextEnteredEvent;

    /// @brief @ref TextEnteredEvent listener.
    using TextEnteredListener = EventListener<TextEnteredEvent>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENTS_LISTENERS_TEXTENTEREDLISTENER_HPP_ */
