#include <gtest/gtest.h>
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/event/events/Event.hpp"
#include "ecstasy/integrations/event/events/KeyEvent.hpp"
#include "ecstasy/integrations/event/events/KeyPressedEvent.hpp"
#include "ecstasy/integrations/event/events/KeyReleasedEvent.hpp"
#include "ecstasy/integrations/event/inputs/Keyboard.hpp"
#include "ecstasy/integrations/event/listeners/KeyCombinationListener.hpp"
#include "ecstasy/integrations/event/listeners/KeyListener.hpp"
#include "ecstasy/integrations/event/listeners/KeySequenceListener.hpp"
#include "ecstasy/integrations/event/listeners/TextEnteredListener.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/storages/MapStorage.hpp"

using namespace ecstasy;
namespace event = ecstasy::integration::event;

TEST(Event, KeyPressed)
{
    Registry registry;
    event::Keyboard &keyboardState = registry.addResource<event::Keyboard>();
    int val1 = 0;
    int val2 = 0;
    int multiEvents = 0;

    /// COVERAGE FOR DEFAULT EVENT, DOES NOTHING
    event::EventsManager::handleEvent(registry, event::Event());

    GTEST_ASSERT_TRUE(keyboardState.isKeyUp(event::Keyboard::Key::A));
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_TRUE(keyboardState.isKeyDown(event::Keyboard::Key::A));

    registry.entityBuilder()
        .with<event::KeyListener>([&val1](Registry &r, Entity e, const event::KeyEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (event.pressed)
                val1++;
        })
        .with<event::EventListeners<event::KeyEvent>>(std::initializer_list<event::EventListener<event::KeyEvent>>{
            [&multiEvents](Registry &r, Entity e, const event::KeyEvent &event) {
                (void)r;
                (void)e;
                (void)event;
                if (event.pressed)
                    multiEvents++;
            },
            [&multiEvents](Registry &r, Entity e, const event::KeyEvent &event) {
                (void)r;
                (void)e;
                (void)event;
                if (event.pressed)
                    multiEvents += 3;
            }})
        .build();
    registry.entityBuilder()
        .with<event::KeyListener>([&val2](Registry &r, Entity e, const event::KeyEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (event.pressed)
                val2--;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::B));
    GTEST_ASSERT_EQ(val1, 1);
    GTEST_ASSERT_EQ(val2, -1);
    GTEST_ASSERT_EQ(multiEvents, 4); /// 1 + 3
}

TEST(Event, KeyReleased)
{
    Registry registry;
    event::Keyboard &keyboardState = registry.addResource<event::Keyboard>();
    int val1 = 0;
    int val2 = 0;

    GTEST_ASSERT_TRUE(keyboardState.isKeyUp(event::Keyboard::Key::A));
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_TRUE(keyboardState.isKeyDown(event::Keyboard::Key::A));

    registry.entityBuilder()
        .with<event::KeyListener>([&val1](Registry &r, Entity e, const event::KeyEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (!event.pressed)
                val1++;
        })
        .build();
    registry.entityBuilder()
        .with<event::KeyListener>([&val2](Registry &r, Entity e, const event::KeyEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (!event.pressed)
                val2--;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::KeyReleasedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_TRUE(keyboardState.isKeyUp(event::Keyboard::Key::A));
    GTEST_ASSERT_EQ(val1, 1);
    GTEST_ASSERT_EQ(val2, -1);
}

TEST(Event, TextEntered)
{
    Registry registry;
    std::uint32_t val = 0;
    int count = 0;

    registry.entityBuilder()
        .with<event::TextEnteredListener>([&val](Registry &r, Entity e, const event::TextEnteredEvent &event) {
            (void)r;
            (void)e;
            GTEST_ASSERT_EQ(event.unicode, val);
        })
        .build();
    registry.entityBuilder()
        .with<event::TextEnteredListener>([&count](Registry &r, Entity e, const event::TextEnteredEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            count++;
        })
        .build();

    int iterations = 50;
    for (int i = 0; i < iterations; i++) {
        event::EventsManager::handleEvent(registry, event::TextEnteredEvent(val));
        val += 12;
    }
    GTEST_ASSERT_EQ(count, iterations);
}

TEST(Event, KeySequence)
{
    Registry registry;
    int sequenceCount = 0;

    /// Sequence ABC listener
    auto &sequenceListener =
        registry.getStorageSafe<event::KeySequenceListener>()
            [registry.entityBuilder()
                    .with<event::KeySequenceListener>(
                        std::initializer_list<event::Keyboard::Key>{
                            event::Keyboard::Key::A, event::Keyboard::Key::B, event::Keyboard::Key::C},
                        [&sequenceCount](Registry &r, Entity e, const event::KeySequenceListener &event) {
                            (void)r;
                            (void)e;
                            (void)event;
                            sequenceCount++;
                        })
                    .build()
                    .getIndex()];
    sequenceListener.setSequence({event::Keyboard::Key::A, event::Keyboard::Key::B, event::Keyboard::Key::C});

    /// Initial state
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedKeys().empty());

    /// Call without force innaccessible
    sequenceListener(registry, registry.getEntity(0));
    GTEST_ASSERT_EQ(sequenceCount, 0);
    sequenceListener(registry, registry.getEntity(0), true);
    GTEST_ASSERT_EQ(sequenceCount, 1);

    /// First sequence key pressed
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::A));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedKeys().empty());
    /// First sequence key released -> key is validated
    event::EventsManager::handleEvent(registry, event::KeyReleasedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::Unknown));
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getValidatedKeys()[0]), static_cast<size_t>(event::Keyboard::Key::A));

    /// Following key without the handleEvent
    GTEST_ASSERT_FALSE(sequenceListener.update(event::KeyPressedEvent(event::Keyboard::Key::B)));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::B));
    GTEST_ASSERT_EQ(sequenceListener.getValidatedKeys().size(), 1);

    GTEST_ASSERT_FALSE(sequenceListener.update(event::KeyReleasedEvent(event::Keyboard::Key::B)));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::Unknown));
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getValidatedKeys()[1]), static_cast<size_t>(event::Keyboard::Key::B));

    /// Final key
    GTEST_ASSERT_FALSE(sequenceListener.update(event::KeyPressedEvent(event::Keyboard::Key::C)));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::C));
    GTEST_ASSERT_EQ(sequenceListener.getValidatedKeys().size(), 2);
    /// Sequence completed and called
    event::EventsManager::handleEvent(registry, event::KeyReleasedEvent(event::Keyboard::Key::C));
    GTEST_ASSERT_EQ(sequenceCount, 2);
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedKeys().empty());

    /// Invalid first key pressed
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::Z));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedKeys().empty());

    /// First sequence key pressed
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::A));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedKeys().empty());

    /// Key pressed while waiting release
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::Z));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedKeys().empty());

    /// First sequence key pressed and released
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::A));
    event::EventsManager::handleEvent(registry, event::KeyReleasedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::Unknown));
    GTEST_ASSERT_FALSE(sequenceListener.getValidatedKeys().empty());

    /// Invalid Key pressed in the sequence
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::Z));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedKeys().empty());

    /// Unhandled release: no held key -> no change
    event::EventsManager::handleEvent(registry, event::KeyReleasedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(
        static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::Unknown));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedKeys().empty());

    /// Unhandled release: key doesn't match the held key -> no change
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::A));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedKeys().empty());
    event::EventsManager::handleEvent(registry, event::KeyReleasedEvent(event::Keyboard::Key::Z));
    GTEST_ASSERT_FALSE(sequenceListener.isComplete());
    GTEST_ASSERT_EQ(static_cast<size_t>(sequenceListener.getHeldKey()), static_cast<size_t>(event::Keyboard::Key::A));
    GTEST_ASSERT_TRUE(sequenceListener.getValidatedKeys().empty());
}

TEST(Event, KeyCombination)
{
    Registry registry;
    int combinationCount = 0;

    /// Sequence ABC listener
    auto &combinationListener =
        registry.getStorageSafe<event::KeyCombinationListener>()
            [registry.entityBuilder()
                    .with<event::KeyCombinationListener>(
                        std::initializer_list<event::Keyboard::Key>{
                            event::Keyboard::Key::A, event::Keyboard::Key::B, event::Keyboard::Key::C},
                        [&combinationCount](Registry &r, Entity e, const event::KeyCombinationListener &event) {
                            (void)r;
                            (void)e;
                            (void)event;
                            combinationCount++;
                        })
                    .build()
                    .getIndex()];
    combinationListener.setCombination({event::Keyboard::Key::A, event::Keyboard::Key::B, event::Keyboard::Key::C});

    /// Initial state
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedKeys(), 0);

    /// Call without force innaccessible
    combinationListener(registry, registry.getEntity(0));
    GTEST_ASSERT_EQ(combinationCount, 0);
    combinationListener(registry, registry.getEntity(0), true);
    GTEST_ASSERT_EQ(combinationCount, 1);

    /// Combination key pressed
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::B));
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedKeys(), 1);

    /// Unhandled key, no changes
    GTEST_ASSERT_FALSE(combinationListener.update(event::KeyPressedEvent(event::Keyboard::Key::Z)));

    /// Same key is pressed again, no changes
    GTEST_ASSERT_FALSE(combinationListener.update(event::KeyPressedEvent(event::Keyboard::Key::B)));
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedKeys(), 1);

    /// Another combination key pressed
    GTEST_ASSERT_FALSE(combinationListener.update(event::KeyPressedEvent(event::Keyboard::Key::A)));
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedKeys(), 2);

    /// A combination key is released
    GTEST_ASSERT_FALSE(combinationListener.update(event::KeyReleasedEvent(event::Keyboard::Key::A)));
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedKeys(), 1);

    /// Missing keys are pressed
    GTEST_ASSERT_FALSE(combinationListener.update(event::KeyPressedEvent(event::Keyboard::Key::A)));
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::C));
    GTEST_ASSERT_EQ(combinationCount, 2);
    GTEST_ASSERT_FALSE(combinationListener.isComplete());
    GTEST_ASSERT_EQ(combinationListener.getValidatedKeys(), 0);
}
