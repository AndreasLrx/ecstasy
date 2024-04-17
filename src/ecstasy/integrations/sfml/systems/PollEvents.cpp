///
/// @file PollEvents.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-16
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include <SFML/Graphics.hpp>

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
    static event::Gamepad::Button getGamepadButton(unsigned int sfmlButton)
    {
        switch (sfmlButton) {
            case 0: return event::Gamepad::Button::FaceDown;
            case 1: return event::Gamepad::Button::FaceRight;
            case 2: return event::Gamepad::Button::FaceLeft;
            case 3: return event::Gamepad::Button::FaceUp;
            case 4: return event::Gamepad::Button::BumperLeft;
            case 5: return event::Gamepad::Button::BumperRight;
            case 6: return event::Gamepad::Button::MiddleLeft;
            case 7: return event::Gamepad::Button::MiddleRight;
            case 8: return event::Gamepad::Button::Middle;
            case 9: return event::Gamepad::Button::ThumbLeft;
            case 10: return event::Gamepad::Button::ThumbRight;

            default: return event::Gamepad::Button::Unknown;
        }
    }

    static event::Gamepad::Axis getGamepadAxis(int sfmlAxis)
    {
        switch (sfmlAxis) {
            case 0: return event::Gamepad::Axis::LeftX;
            case 1: return event::Gamepad::Axis::LeftY;
            case 2: return event::Gamepad::Axis::TriggerLeft;
            case 3: return event::Gamepad::Axis::TriggerRight;
            case 4: return event::Gamepad::Axis::RightX;
            case 5: return event::Gamepad::Axis::RightY;
            case 6: return event::Gamepad::Axis::DPadX;
            case 7: return event::Gamepad::Axis::DPadY;

            default: return event::Gamepad::Axis::Unknown;
        }
    }

    void PollEvents::run(ecstasy::Registry &registry)
    {
        if (!registry.hasResource<RenderWindow>())
            return;
        RR<RenderWindow> windowWrapper = registry.getResource<RenderWindow>();

        sf::Event event;
        while (windowWrapper->get().pollEvent(event)) {
            switch (event.type) {
                /// Mouse events
                case sf::Event::MouseButtonPressed:
                case sf::Event::MouseButtonReleased:
                    event::EventsManager::handleEvent(registry,
                        event::MouseButtonEvent(static_cast<event::Mouse::Button>(event.mouseButton.button),
                            event.type == sf::Event::MouseButtonPressed));
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
                case sf::Event::KeyReleased:
                    event::EventsManager::handleEvent(registry,
                        event::KeyEvent(
                            static_cast<event::Keyboard::Key>(event.key.code), event.type == sf::Event::KeyPressed));
                    break;
                case sf::Event::TextEntered:
                    event::EventsManager::handleEvent(registry, event::TextEnteredEvent(event.text.unicode));
                    break;

                /// Gamepad events
                case sf::Event::JoystickButtonPressed:
                case sf::Event::JoystickButtonReleased:
                    event::EventsManager::handleEvent(registry,
                        event::GamepadButtonEvent(event.joystickButton.joystickId,
                            getGamepadButton(event.joystickButton.button),
                            event.type == sf::Event::JoystickButtonPressed));
                    break;
                case sf::Event::JoystickConnected:
                case sf::Event::JoystickDisconnected:
                    event::EventsManager::handleEvent(registry,
                        event::GamepadConnectedEvent(
                            event.joystickConnect.joystickId, event.type == sf::Event::JoystickConnected));
                    break;
                case sf::Event::JoystickMoved:
                    event::EventsManager::handleEvent(registry,
                        event::GamepadAxisEvent(event.joystickMove.joystickId, getGamepadAxis(event.joystickMove.axis),
                            event.joystickMove.position / 100.f));
                    break;

                case sf::Event::Closed: windowWrapper->get().close(); break;
                default: break;
            }
        }
    }
} // namespace ecstasy::integration::sfml
