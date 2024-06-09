# SFML integration

@tableofcontents

## Description

The [sfml integration](@ref ecstasy::integration::sfml) contains helpers to use ecstasy with the SFML graphic library.
For the moment it doesn't contain much but it have two [demos](@ref src/integrations/sfml/demo) showing how to create a `sf::RenderWindow` and handle its events.

@warning This integration requires the event integration

## Usage

The most basic usage of the SFML integration is the following:

- Create a `sf::RenderWindow` as a registry resource
- Add the [PollEvents](@ref ecstasy::integration::sfml::PollEvents) system, fetching the events from the window and sending them to the event integration (therefore calling the listeners and updating the input resources if any)
- Run your program loop and don't forget to run the PollEvents system, otherwise the close button will not work!

@note You should register event or actions listeners to have some interractivity !
See the [events demo](@ref src/ecstasy/integrations/sfml/demo/02_events/main.cpp)

```cpp

namespace esf = ecstasy::integration::sfml;

int main(int argc, char **argv)
{
    ecstasy::Registry registry;
    // Create the window
    esf::RenderWindow &window =
        registry.addResource<esf::RenderWindow>(sf::VideoMode(1280, 720), "ECSTASY SFML integration: basic");
    // Add the events fetching
    registry.addSystem<esf::PollEvents>();
    (void)argv;
    (void)argc;

    // Program loop
    while (window.get().isOpen()) {
        window.get().clear(sf::Color::White);
        // Run the PollEvents system. If the close button is pressed the window will be closed and the while condition will be evaluated to false
        registry.runSystems();
        window.get().display();
    }
    return 0;
}

```
