#include <gtest/gtest.h>
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/event/events/Event.hpp"
#include "ecstasy/integrations/event/events/KeyEvent.hpp"
#include "ecstasy/integrations/event/events/KeyPressedEvent.hpp"
#include "ecstasy/integrations/event/events/KeyReleasedEvent.hpp"
#include "ecstasy/integrations/event/inputs/Keyboard.hpp"
#include "ecstasy/integrations/event/listeners/KeyListener.hpp"
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
