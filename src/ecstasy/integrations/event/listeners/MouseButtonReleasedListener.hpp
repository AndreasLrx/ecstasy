///
/// @file MouseButtonReleasedListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-06
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONRELEASEDLISTENER_HPP_
#define ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONRELEASEDLISTENER_HPP_

#include <functional>
#include "ecstasy/integrations/event/events/MouseButtonReleasedEvent.hpp"
#include "ecstasy/resources/entity/Entity.hpp"

namespace ecstasy
{
    class Registry;
} // namespace ecstasy

namespace ecstasy::integration::event
{
    using MouseButtonReleasedListener = std::function<void(Registry &, Entity, const MouseButtonReleasedEvent &)>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENTS_LISTENERS_MOUSEBUTTONRELEASEDLISTENER_HPP_ */
