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

#include "ecstasy/registry/Registry.hpp"

int main(int argc, char **argv)
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "ECSTASY SFML integration");
    ecstasy::Registry registry;
    (void)argv;

    sf::Event e;
    while (window.isOpen()) {
        window.clear(sf::Color::White);
        while (window.pollEvent(e))
            if (e.type == sf::Event::Closed) {
                window.close();
                break;
            }
        window.display();
    }
    return argc;
}