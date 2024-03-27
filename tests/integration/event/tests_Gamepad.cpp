#include <gtest/gtest.h>
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/event/events/Event.hpp"
#include "ecstasy/integrations/event/events/GamepadAxisEvent.hpp"
#include "ecstasy/integrations/event/events/GamepadButtonPressedEvent.hpp"
#include "ecstasy/integrations/event/events/GamepadButtonReleasedEvent.hpp"
#include "ecstasy/integrations/event/inputs/Gamepads.hpp"
#include "ecstasy/integrations/event/listeners/GamepadAxisListener.hpp"
#include "ecstasy/integrations/event/listeners/GamepadButtonListener.hpp"
#include "ecstasy/integrations/event/listeners/GamepadCombinationListener.hpp"
#include "ecstasy/integrations/event/listeners/GamepadConnectedListener.hpp"
#include "ecstasy/integrations/event/listeners/GamepadSequenceListener.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/storages/MapStorage.hpp"

using namespace ecstasy;
namespace event = ecstasy::integration::event;

TEST(Event, GamepadButtonPressed)
{
    Registry registry;
    int val1 = 0;
    int val2 = 0;

    registry.entityBuilder()
        .with<event::GamepadButtonListener>([&val1](Registry &r, Entity e, const event::GamepadButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (event.pressed)
                val1++;
        })
        .build();
    registry.entityBuilder()
        .with<event::GamepadButtonListener>([&val2](Registry &r, Entity e, const event::GamepadButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (event.pressed)
                val2--;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));

    event::Gamepads &gamepadsState = registry.addResource<event::Gamepads>();
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonUp(event::Gamepad::Button::FaceDown));
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonDown(event::Gamepad::Button::FaceDown));

    GTEST_ASSERT_EQ(val1, 2);
    GTEST_ASSERT_EQ(val2, -2);
}

TEST(Event, GamepadButtonReleased)
{
    Registry registry;
    event::Gamepads &gamepadsState = registry.addResource<event::Gamepads>();
    int val1 = 0;
    int val2 = 0;

    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonUp(event::Gamepad::Button::FaceDown));
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonDown(event::Gamepad::Button::FaceDown));

    registry.entityBuilder()
        .with<event::GamepadButtonListener>([&val1](Registry &r, Entity e, const event::GamepadButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (!event.pressed)
                val1++;
        })
        .build();
    registry.entityBuilder()
        .with<event::GamepadButtonListener>([&val2](Registry &r, Entity e, const event::GamepadButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (!event.pressed)
                val2--;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::GamepadButtonReleasedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonUp(event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_EQ(val1, 1);
    GTEST_ASSERT_EQ(val2, -1);
}

TEST(Event, GamepadConnected)
{
    Registry registry;
    bool connected = false;

    registry.entityBuilder()
        .with<event::GamepadConnectedListener>(
            [&connected](Registry &r, Entity e, const event::GamepadConnectedEvent &event) {
                (void)r;
                (void)e;
                connected = event.connected;
            })
        .build();
    GTEST_ASSERT_FALSE(connected);
    event::EventsManager::handleEvent(registry, event::GamepadConnectedEvent(0, true));
    GTEST_ASSERT_TRUE(connected);

    event::Gamepads &gamepadsState = registry.addResource<event::Gamepads>();
    GTEST_ASSERT_FALSE(gamepadsState.get(0).isConnected());
    event::EventsManager::handleEvent(registry, event::GamepadConnectedEvent(0, true));
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isConnected());
    event::EventsManager::handleEvent(registry, event::GamepadConnectedEvent(0, false));
    GTEST_ASSERT_FALSE(gamepadsState.get(0).isConnected());
}

TEST(Event, GamepadAxis)
{
    Registry registry;
    float value = 0.f;

    registry.entityBuilder()
        .with<event::GamepadAxisListener>([&value](Registry &r, Entity e, const event::GamepadAxisEvent &event) {
            (void)r;
            (void)e;
            value = event.value;
        })
        .build();
    GTEST_ASSERT_EQ(value, 0.f);
    event::EventsManager::handleEvent(registry, event::GamepadAxisEvent(0, event::Gamepad::Axis::LeftX, 0.6f));
    GTEST_ASSERT_EQ(value, 0.6f);

    event::Gamepads &gamepadsState = registry.addResource<event::Gamepads>();
    GTEST_ASSERT_EQ(gamepadsState.get(0).getAxisValue(event::Gamepad::Axis::TriggerLeft), -1.f);
    event::EventsManager::handleEvent(registry, event::GamepadAxisEvent(0, event::Gamepad::Axis::TriggerLeft, 0.3f));
    GTEST_ASSERT_EQ(gamepadsState.get(0).getAxisValue(event::Gamepad::Axis::TriggerLeft), 0.3f);
}

TEST(Event, GamepadSequence)
{
    Registry registry;
    int sequenceCount = 0;

    /// Sequence ABC listener
    auto &sequenceListener =
        registry.getStorageSafe<event::GamepadSequenceListener>()
            [registry.entityBuilder()
                    .with<event::GamepadSequenceListener>(
                        std::initializer_list<event::Gamepad::Button>{event::Gamepad::Button::FaceDown,
                            event::Gamepad::Button::FaceRight, event::Gamepad::Button::FaceUp},
                        [&sequenceCount](Registry &r, Entity e, const event::GamepadSequenceListener &event) {
                            (void)r;
                            (void)e;
                            (void)event;
                            sequenceCount++;
                        })
                    .build()
                    .getIndex()];
    sequenceListener.setSequence(
        {event::Gamepad::Button::FaceDown, event::Gamepad::Button::FaceRight, event::Gamepad::Button::FaceUp});

    /// Initial state
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedButtons().empty());

    /// Call without force innaccessible
    sequenceListener(registry, registry.getEntity(0));
    GTEST_ASSERT_EQ(sequenceCount, 0);
    sequenceListener(registry, registry.getEntity(0), true);
    GTEST_ASSERT_EQ(sequenceCount, 1);

    /// First sequence key pressed
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedButtons().empty());
    /// First sequence key released -> key is validated
    event::EventsManager::handleEvent(registry, event::GamepadButtonReleasedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::Unknown));
    GTEST_ASSERT_EQ(static_cast<size_t>(sequenceListener.getValidatedButtons()[0]),
        static_cast<size_t>(event::Gamepad::Button::FaceDown));

    /// Following key without the handleEvent
    GTEST_ASSERT_FALSE(sequenceListener.update(event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceRight)));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::FaceRight));
    GTEST_ASSERT_EQ(sequenceListener.getValidatedButtons().size(), 1);

    GTEST_ASSERT_FALSE(
        sequenceListener.update(event::GamepadButtonReleasedEvent(0, event::Gamepad::Button::FaceRight)));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::Unknown));
    GTEST_ASSERT_EQ(static_cast<size_t>(sequenceListener.getValidatedButtons()[1]),
        static_cast<size_t>(event::Gamepad::Button::FaceRight));

    /// Final key
    GTEST_ASSERT_FALSE(sequenceListener.update(event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceUp)));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::FaceUp));
    GTEST_ASSERT_EQ(sequenceListener.getValidatedButtons().size(), 2);
    /// Sequence completed and called
    event::EventsManager::handleEvent(registry, event::GamepadButtonReleasedEvent(0, event::Gamepad::Button::FaceUp));
    GTEST_ASSERT_EQ(sequenceCount, 2);
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedButtons().empty());

    /// Invalid first key pressed
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceLeft));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedButtons().empty());

    /// First sequence key pressed
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedButtons().empty());

    /// Key pressed while waiting release
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceLeft));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedButtons().empty());

    /// First sequence key pressed and released
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));
    event::EventsManager::handleEvent(registry, event::GamepadButtonReleasedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::Unknown));
    GTEST_ASSERT_FALSE(sequenceListener.getValidatedButtons().empty());

    /// Invalid Key pressed in the sequence
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceLeft));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedButtons().empty());

    /// Unhandled release: no held key -> no change
    event::EventsManager::handleEvent(registry, event::GamepadButtonReleasedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedButtons().empty());

    /// Unhandled release: key doesn't match the held key -> no change
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedButtons().empty());
    event::EventsManager::handleEvent(registry, event::GamepadButtonReleasedEvent(0, event::Gamepad::Button::FaceLeft));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldButton()), static_cast<size_t>(event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedButtons().empty());
}

TEST(Event, GamepadCombination)
{
    Registry registry;
    int combinationCount = 0;

    /// Sequence ABC listener
    auto &combinationListener =
        registry.getStorageSafe<event::GamepadCombinationListener>()
            [registry.entityBuilder()
                    .with<event::GamepadCombinationListener>(
                        std::initializer_list<event::Gamepad::Button>{event::Gamepad::Button::FaceDown,
                            event::Gamepad::Button::FaceRight, event::Gamepad::Button::FaceUp},
                        [&combinationCount](Registry &r, Entity e, const event::GamepadCombinationListener &event) {
                            (void)r;
                            (void)e;
                            (void)event;
                            combinationCount++;
                        })
                    .build()
                    .getIndex()];
    combinationListener.setCombination(
        {event::Gamepad::Button::FaceDown, event::Gamepad::Button::FaceRight, event::Gamepad::Button::FaceUp});

    /// Initial state
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedButtons(), 0);

    /// Call without force innaccessible
    combinationListener(registry, registry.getEntity(0));
    GTEST_ASSERT_EQ(combinationCount, 0);
    combinationListener(registry, registry.getEntity(0), true);
    GTEST_ASSERT_EQ(combinationCount, 1);

    /// Combination key pressed
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceRight));
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedButtons(), 1);

    /// Unhandled key, no changes
    GTEST_ASSERT_FALSE(
        combinationListener.update(event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceLeft)));

    /// Same key is pressed again, no changes
    GTEST_ASSERT_FALSE(
        combinationListener.update(event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceRight)));
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedButtons(), 1);

    /// Another combination key pressed
    GTEST_ASSERT_FALSE(
        combinationListener.update(event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown)));
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedButtons(), 2);

    /// A combination key is released
    GTEST_ASSERT_FALSE(
        combinationListener.update(event::GamepadButtonReleasedEvent(0, event::Gamepad::Button::FaceDown)));
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedButtons(), 1);

    /// Missing keys are pressed
    GTEST_ASSERT_FALSE(
        combinationListener.update(event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown)));
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceUp));
    GTEST_ASSERT_EQ(combinationCount, 2);
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedButtons(), 0);
}
