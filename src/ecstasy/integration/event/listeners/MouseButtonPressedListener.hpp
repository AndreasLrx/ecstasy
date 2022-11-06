///
/// @file MouseButtonPressedListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-06
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONPRESSEDLISTENER_HPP_
#define ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONPRESSEDLISTENER_HPP_

#include <functional>
#include "ecstasy/integration/event/events/MouseButtonPressedEvent.hpp"

namespace ecstasy
{
    class Registry;
    class Entity;
} // namespace ecstasy

namespace ecstasy::integration::event
{
    using MouseButtonPressedListener = std::function<bool(Registry &, Entity &, const MouseButtonPressedEvent &)>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONPRESSEDLISTENER_HPP_ */
