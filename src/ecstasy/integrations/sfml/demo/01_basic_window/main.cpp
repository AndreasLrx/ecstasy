///
/// @file main.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-15
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include <SFML/Graphics.hpp>
#include <iostream>

#include "ecstasy/integrations/sfml/resources/RenderWindow.hpp"
#include "ecstasy/integrations/sfml/systems/PollEvents.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/storages/MapStorage.hpp"

#include "ecstasy/integrations/event/listeners/MouseButtonListener.hpp"

namespace esf = ecstasy::integration::sfml;
namespace event = ecstasy::integration::event;

static void addEventListeners(ecstasy::Registry &registry)
{
    registry.entityBuilder()
        .with<event::MouseButtonListener>(
            [](ecstasy::Registry &r, ecstasy::Entity entity, const event::MouseButtonEvent &e) {
                (void)r;
                (void)entity;
                std::cout << "Mouse button " << static_cast<int>(e.button) << " event, pressed = " << e.pressed
                          << std::endl;
            })
        .build();
}

int main(int argc, char **argv)
{
    ecstasy::Registry registry;
    esf::RenderWindow &window =
        registry.addResource<esf::RenderWindow>(sf::VideoMode(1280, 720), "ECSTASY SFML integration");
    registry.addSystem<esf::PollEvents>();
    (void)argv;

    addEventListeners(registry);

    while (window.get().isOpen()) {
        window.get().clear(sf::Color::White);
        registry.runSystems();
        window.get().display();
    }
    return argc;
}