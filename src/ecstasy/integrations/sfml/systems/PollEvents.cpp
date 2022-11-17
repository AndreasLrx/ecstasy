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
#include "ecstasy/integrations/event/events/KeyPressedEvent.hpp"
#include "ecstasy/integrations/event/events/KeyReleasedEvent.hpp"
#include "ecstasy/integrations/event/events/MouseButtonPressedEvent.hpp"
#include "ecstasy/integrations/event/events/MouseButtonReleasedEvent.hpp"
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
                /// Mouse events
                case sf::Event::MouseButtonPressed:
                    event::EventsManager::handleEvent(registry,
                        event::MouseButtonPressedEvent(static_cast<event::Mouse::Button>(event.mouseButton.button)));
                    break;
                case sf::Event::MouseButtonReleased:
                    event::EventsManager::handleEvent(registry,
                        event::MouseButtonReleasedEvent(static_cast<event::Mouse::Button>(event.mouseButton.button)));
                    break;
                case sf::Event::MouseWheelScrolled:
                    event::EventsManager::handleEvent(registry,
                        event::MouseWheelScrollEvent(event.mouseWheelScroll.delta,
                            static_cast<event::Mouse::Wheel>(event.mouseWheelScroll.wheel)));
                    break;
                case sf::Event::MouseMoved:
                    event::EventsManager::handleEvent(
                        registry, event::MouseMoveEvent(event.mouseMove.x, event.mouseMove.y));
                    break;

                /// Keyboard events
                case sf::Event::KeyPressed:
                    event::EventsManager::handleEvent(
                        registry, event::KeyPressedEvent(static_cast<event::Keyboard::Key>(event.key.code)));
                    break;
                case sf::Event::KeyReleased:
                    event::EventsManager::handleEvent(
                        registry, event::KeyReleasedEvent(static_cast<event::Keyboard::Key>(event.key.code)));
                    break;

                case sf::Event::Closed: windowWrapper.get().close(); break;
                default: break;
            }
        }
    }
} // namespace ecstasy::integration::sfml