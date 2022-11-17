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

#include <functional>
#include "ecstasy/resources/entity/Entity.hpp"

namespace ecstasy
{
    class Registry;
} // namespace ecstasy

namespace ecstasy::integration::event
{
    struct TextEnteredEvent;

    using TextEnteredListener = std::function<void(Registry &, Entity, const TextEnteredEvent &)>;
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENTS_LISTENERS_TEXTENTEREDLISTENER_HPP_ */
