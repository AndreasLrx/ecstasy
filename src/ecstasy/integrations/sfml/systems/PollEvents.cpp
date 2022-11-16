///
/// @file PollEvents.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-16
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include <SFML/Graphics.hpp>
#include <iostream>

#include "PollEvents.hpp"
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/event/events/Event.hpp"
#include "ecstasy/integrations/event/events/MouseButtonEvent.hpp"
#include "ecstasy/integrations/sfml/resources/RenderWindow.hpp"
#include "ecstasy/registry/Registry.hpp"

namespace event = ecstasy::integration::event;

namespace ecstasy::integration::sfml
{
    void PollEvents::run(ecstasy::Registry &registry)
    {
        if (!registry.hasResource<RenderWindow>())
            return;
        RenderWindow &windowWrapper = registry.getResource<RenderWindow>();

        sf::Event event;
        while (windowWrapper.get().pollEvent(event)) {
            switch (event.type) {
                case sf::Event::MouseButtonPressed:
                    event::EventsManager::handleEvent(registry,
                        event::MouseButtonEvent(static_cast<event::Mouse::Button>(event.mouseButton.button), true));
                    break;
                case sf::Event::Closed: windowWrapper.get().close(); break;
                default: break;
            }
        }
    }
} // namespace ecstasy::integration::sfml