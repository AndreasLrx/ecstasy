///
/// @file KeyListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYLISTENER_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYLISTENER_HPP_

#include <functional>
#include "ecstasy/resources/entity/Entity.hpp"

namespace ecstasy
{
    class Registry;
} // namespace ecstasy

namespace ecstasy::integration::event
{
    struct KeyEvent;

    using KeyListener = std::function<void(Registry &, Entity, const KeyEvent &)>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYLISTENER_HPP_ */
