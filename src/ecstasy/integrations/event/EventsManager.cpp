///
/// @file EventsManager.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-06
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "EventsManager.hpp"
#include "ecstasy/config.hpp"
#include "ecstasy/integrations/event/listeners/EventListeners.hpp"
#include "ecstasy/query/Query.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/registry/modifiers/Maybe.hpp"
#include "ecstasy/registry/modifiers/Or.hpp"
#include "ecstasy/resources/entity/Entities.hpp"
#include "ecstasy/storages/MapStorage.hpp"
#include "events/Event.hpp"
#include "inputs/Gamepads.hpp"
#include "listeners/GamepadCombinationListener.hpp"
#include "listeners/GamepadSequenceListener.hpp"
#include "listeners/KeyCombinationListener.hpp"
#include "listeners/KeySequenceListener.hpp"

#ifdef ECSTASY_INTEGRATIONS_USER_ACTION
    #include "ecstasy/integrations/user_action/Users.hpp"
#endif

namespace ecstasy::integration::event
{
    template <typename E>
    static void callListeners(Registry &registry, const E &event)
    {
        for (auto [entity, listener, listeners] :
            registry.select<Entities, Maybe<EventListener<E>>, Maybe<EventListeners<E>>>()
                .template where<Or<EventListener<E>, EventListeners<E>>>()) {
            if (listener)
                listener.value()(registry, entity, event);
            if (listeners)
                listeners.value()(registry, entity, event);
        }
    }

    static void callKeyListeners(Registry &registry, const KeyEvent &event)
    {
        for (auto [entity, listener, listeners, sequence, combination] :
            registry
                .select<Entities, Maybe<EventListener<KeyEvent>>, Maybe<EventListeners<KeyEvent>>,
                    Maybe<KeySequenceListener>, Maybe<KeyCombinationListener>>()
                .where<Or<EventListener<KeyEvent>, EventListeners<KeyEvent>, KeySequenceListener,
                    KeyCombinationListener>>()) {
            if (listener)
                listener.value()(registry, entity, event);
            if (listeners)
                listeners.value()(registry, entity, event);
            if (sequence && sequence.value().get().update(event))
                sequence.value()(registry, entity);
            if (combination && combination.value().get().update(event))
                combination.value()(registry, entity);
        }
    }

    static void callGamepadListeners(Registry &registry, const GamepadButtonEvent &event)
    {
        for (auto [entity, listener, listeners, sequence, combination] :
            registry
                .select<Entities, Maybe<EventListener<GamepadButtonEvent>>, Maybe<EventListeners<GamepadButtonEvent>>,
                    Maybe<GamepadSequenceListener>, Maybe<GamepadCombinationListener>>()
                .where<Or<EventListener<GamepadButtonEvent>, EventListeners<GamepadButtonEvent>,
                    GamepadSequenceListener, GamepadCombinationListener>>()) {
            if (listener)
                listener.value()(registry, entity, event);
            if (listeners)
                listeners.value()(registry, entity, event);
            if (sequence && sequence.value().get().update(event))
                sequence.value()(registry, entity);
            if (combination && combination.value().get().update(event))
                combination.value()(registry, entity);
        }
    }

    void EventsManager::handleEvent(Registry &registry, const Event &event)
    {
        switch (event.type) {
            case Event::Type::MouseButtonPressed:
            case Event::Type::MouseButtonReleased:
                callListeners(registry, event.mouseButton);

                if (registry.hasResource<Mouse>())
                    registry.getResource<Mouse>().get().setButtonState(
                        event.mouseButton.button, event.mouseButton.pressed);
                break;
            case Event::Type::MouseWheelScrolled: callListeners(registry, event.mouseWheel); break;
            case Event::Type::MouseMoved:
                callListeners(registry, event.mouseMove);

                if (registry.hasResource<Mouse>()) {
                    RR<Mouse> mouse = registry.getResource<Mouse>();
                    mouse.get().setPosition(
                        event.mouseMove.x + mouse.get().getX(), event.mouseMove.y + mouse.get().getY());
                }
                break;
            case Event::Type::KeyPressed:
            case Event::Type::KeyReleased:
                callKeyListeners(registry, event.key);

                if (registry.hasResource<Keyboard>())
                    registry.getResource<Keyboard>().get().setKeyState(event.key.key, event.key.pressed);
                break;
            case Event::Type::TextEntered: callListeners(registry, event.text); break;
            case Event::Type::GamepadButtonPressed:
            case Event::Type::GamepadButtonReleased:
                callGamepadListeners(registry, event.gamepadButton);

                if (registry.hasResource<Gamepads>())
                    registry.getResource<Gamepads>()
                        .get()
                        .get(event.gamepadButton.id)
                        .setButtonState(event.gamepadButton.button, event.gamepadButton.pressed);
                break;
            case Event::Type::GamepadConnected:
            case Event::Type::GamepadDisconnected:
                callListeners(registry, event.gamepadConnected);

                if (registry.hasResource<Gamepads>())
                    registry.getResource<Gamepads>()
                        .get()
                        .get(event.gamepadConnected.id)
                        .setConnected(event.gamepadConnected.connected);
                break;
            case Event::Type::GamepadAxis:
                callListeners(registry, event.gamepadAxis);

                if (registry.hasResource<Gamepads>())
                    registry.getResource<Gamepads>()
                        .get()
                        .get(event.gamepadAxis.id)
                        .setAxisValue(event.gamepadAxis.axis, event.gamepadAxis.value);
                break;
            default: break;
        }

#ifdef ECSTASY_INTEGRATIONS_USER_ACTION
        if (registry.hasResource<user_action::Users>())
            registry.getResource<const user_action::Users>().get().handleEvent(registry, event);
#endif
    }
} // namespace ecstasy::integration::event
