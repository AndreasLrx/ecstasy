///
/// @file MouseButtonListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-06
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONLISTENER_HPP_
#define ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONLISTENER_HPP_

#include <functional>
#include "ecstasy/integrations/event/events/MouseButtonEvent.hpp"
#include "ecstasy/resources/entity/Entity.hpp"

namespace ecstasy
{
    class Registry;
} // namespace ecstasy

namespace ecstasy::integration::event
{
    using MouseButtonListener = std::function<void(Registry &, Entity, const MouseButtonEvent &)>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONLISTENER_HPP_ */
