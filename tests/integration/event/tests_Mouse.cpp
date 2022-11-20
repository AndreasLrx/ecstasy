#include <gtest/gtest.h>
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/event/events/Event.hpp"
#include "ecstasy/integrations/event/events/MouseButtonPressedEvent.hpp"
#include "ecstasy/integrations/event/events/MouseButtonReleasedEvent.hpp"
#include "ecstasy/integrations/event/inputs/Mouse.hpp"
#include "ecstasy/integrations/event/listeners/MouseButtonListener.hpp"
#include "ecstasy/integrations/event/listeners/MouseMoveListener.hpp"
#include "ecstasy/integrations/event/listeners/MouseWheelScrollListener.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/storages/MapStorage.hpp"

using namespace ecstasy;
namespace event = ecstasy::integration::event;

TEST(Event, MouseButtonPressed)
{
    Registry registry;
    int val2 = 0;
    int multiEvents = 0;

    registry.entityBuilder()
        .with<event::EventListeners<event::MouseButtonEvent>>(
            std::initializer_list<event::EventListener<event::MouseButtonEvent>>{
                [&multiEvents](Registry &r, Entity e, const event::MouseButtonEvent &event) {
                    (void)r;
                    (void)e;
                    (void)event;
                    if (event.pressed)
                        multiEvents++;
                },
                [&multiEvents](Registry &r, Entity e, const event::MouseButtonEvent &event) {
                    (void)r;
                    (void)e;
                    (void)event;
                    if (event.pressed)
                        multiEvents += 3;
                }})
        .build();
    registry.entityBuilder()
        .with<event::MouseButtonListener>([&val2](Registry &r, Entity e, const event::MouseButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (event.pressed)
                val2--;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::MouseButtonPressedEvent(event::Mouse::Button::Left));

    event::Mouse &mouseState = registry.addResource<event::Mouse>();
    GTEST_ASSERT_TRUE(mouseState.isButtonUp(event::Mouse::Button::Left));
    event::EventsManager::handleEvent(registry, event::MouseButtonPressedEvent(event::Mouse::Button::Left));
    GTEST_ASSERT_TRUE(mouseState.isButtonDown(event::Mouse::Button::Left));

    GTEST_ASSERT_EQ(val2, -2);
    GTEST_ASSERT_EQ(multiEvents, 8); /// 1 + 3
}

TEST(Event, MouseButtonReleased)
{
    Registry registry;
    int val1 = 0;
    int val2 = 0;

    registry.entityBuilder()
        .with<event::MouseButtonListener>([&val1](Registry &r, Entity e, const event::MouseButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (!event.pressed)
                val1++;
        })
        .build();
    registry.entityBuilder()
        .with<event::MouseButtonListener>([&val2](Registry &r, Entity e, const event::MouseButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (!event.pressed)
                val2--;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::MouseButtonReleasedEvent(event::Mouse::Button::Left));

    event::Mouse &mouseState = registry.addResource<event::Mouse>();
    GTEST_ASSERT_TRUE(mouseState.isButtonUp(event::Mouse::Button::Left));
    event::EventsManager::handleEvent(registry, event::MouseButtonPressedEvent(event::Mouse::Button::Left));
    GTEST_ASSERT_TRUE(mouseState.isButtonDown(event::Mouse::Button::Left));
    event::EventsManager::handleEvent(registry, event::MouseButtonReleasedEvent(event::Mouse::Button::Left));
    GTEST_ASSERT_TRUE(mouseState.isButtonUp(event::Mouse::Button::Left));

    GTEST_ASSERT_EQ(val1, 2);
    GTEST_ASSERT_EQ(val2, -2);
}

TEST(Event, MouseWheelScroll)
{
    Registry registry;
    float val1 = 0;
    float val2 = 0;

    event::EventsManager::handleEvent(registry, event::MouseWheelScrollEvent(5));
    GTEST_ASSERT_EQ(val1, val2);
    GTEST_ASSERT_EQ(val1, 0);

    registry.entityBuilder()
        .with<event::MouseWheelScrollListener>(
            [&val1](Registry &r, Entity e, const event::MouseWheelScrollEvent &event) {
                (void)r;
                (void)e;
                val1 += event.delta;
            })
        .build();
    registry.entityBuilder()
        .with<event::MouseWheelScrollListener>(
            [&val2](Registry &r, Entity e, const event::MouseWheelScrollEvent &event) {
                (void)r;
                (void)e;
                (void)event;
                val2 -= event.delta;
            })
        .build();

    event::EventsManager::handleEvent(registry, event::MouseWheelScrollEvent(5));
    GTEST_ASSERT_EQ(val1, 5);
    GTEST_ASSERT_EQ(val2, -5);
    event::EventsManager::handleEvent(registry, event::MouseWheelScrollEvent(-2));
    GTEST_ASSERT_EQ(val1, 3);
    GTEST_ASSERT_EQ(val2, -3);
}

TEST(Event, MouseMoved)
{
    Registry registry;
    int val1 = 0;
    int val2 = 0;

    registry.entityBuilder()
        .with<event::MouseMoveListener>([&val1](Registry &r, Entity e, const event::MouseMoveEvent &event) {
            (void)r;
            (void)e;
            val1 += event.x;
        })
        .build();
    registry.entityBuilder()
        .with<event::MouseMoveListener>([&val2](Registry &r, Entity e, const event::MouseMoveEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            val2 -= event.y;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::MouseMoveEvent(10, -20));

    event::Mouse &mouseState = registry.addResource<event::Mouse>();
    GTEST_ASSERT_EQ(mouseState.getPosition(), std::make_pair(0, 0));
    event::EventsManager::handleEvent(registry, event::MouseMoveEvent(42, 84));
    GTEST_ASSERT_EQ(mouseState.getPosition(), std::make_pair(42, 84));
    GTEST_ASSERT_EQ(val1, 52);
    GTEST_ASSERT_EQ(val2, -64);
}