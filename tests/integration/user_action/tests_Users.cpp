#include <gtest/gtest.h>
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/user_action/ActionListener.hpp"
#include "ecstasy/integrations/user_action/Users.hpp"

namespace event = ecstasy::integration::event;
namespace user_action = ecstasy::integration::user_action;

namespace ecstasy::integration::user_action
{
    class UsersTester {
      public:
        static void removeOutdatedBindings()
        {
            user_action::Users users(2);

            /// First case: binding is effective in a UserProfile
            users.getUserProfile().getActionBindings().getBindings().emplace_back(0, event::Mouse::Button::Left);
            users._mouseButtonToAction.insert({event::Mouse::Button::Left, {0, 0}});
            /// Second case: Binding is affected to a user not existing
            users._mouseButtonToAction.insert({event::Mouse::Button::Left, {0, 5}});

            /// The second one is removed
            GTEST_ASSERT_EQ(users._mouseButtonToAction.size(), 2);
            users.removeOutdatedBindings(users._mouseButtonToAction);
            GTEST_ASSERT_EQ(users._mouseButtonToAction.size(), 1);

            /// Third case: Binding is no more linked to a user action
            users.getUserProfile().getActionBindings().getBindings().pop_back();

            /// The third one is removed because the user doesn't have it anymore
            users.removeOutdatedBindings(users._mouseButtonToAction);
            GTEST_ASSERT_TRUE(users._mouseButtonToAction.empty());
        }

        static void addBindingIfMissing()
        {
            user_action::Users users(2);

            GTEST_ASSERT_TRUE(users._keyToAction.empty());
            /// First iteration: no binding are present -> binding is add
            /// Second iteration: binding already present -> skipped
            for (int i = 0; i < 2; i++) {
                users.addBindingIfMissing(users.getUserProfile(), ActionBinding{0, event::Keyboard::Key::D},
                    event::Keyboard::Key::D, users._keyToAction);
                GTEST_ASSERT_EQ(users._keyToAction.size(), 1);
            }

            /// Third case: action id is different -> add
            users.addBindingIfMissing(users.getUserProfile(), ActionBinding{1, event::Keyboard::Key::D},
                event::Keyboard::Key::D, users._keyToAction);
            GTEST_ASSERT_EQ(users._keyToAction.size(), 2);

            /// Fourth case: user id is different -> add
            users.addBindingIfMissing(users.getUserProfile(1), ActionBinding{1, event::Keyboard::Key::D},
                event::Keyboard::Key::D, users._keyToAction);
            GTEST_ASSERT_EQ(users._keyToAction.size(), 3);
        }

        static void updateBindings()
        {
            Users users(2);

            auto &binds = users.getUserProfile().getActionBindings().getBindings();

            GTEST_ASSERT_EQ(users._mouseButtonToAction.size(), 0);
            GTEST_ASSERT_EQ(users._keyToAction.size(), 0);
            GTEST_ASSERT_EQ(users._gamepadButtonToAction.size(), 0);
            GTEST_ASSERT_EQ(users._gamepadAxisToAction.size(), 0);

            /// One binding of each type + one invalid
            binds.emplace_back(0, event::Mouse::Button::Left);
            binds.emplace_back(0, event::Keyboard::Key::G);
            binds.emplace_back(0, event::Gamepad::Button::BumperLeft);
            binds.emplace_back(0, event::Gamepad::Axis::TriggerLeft);
            binds.emplace_back();

            users.updateBindings();
            GTEST_ASSERT_EQ(users._mouseButtonToAction.size(), 1);
            GTEST_ASSERT_EQ(users._keyToAction.size(), 1);
            GTEST_ASSERT_EQ(users._gamepadButtonToAction.size(), 1);
            GTEST_ASSERT_EQ(users._gamepadAxisToAction.size(), 1);

            /// Remove two (gamepad axis and empty)
            binds.pop_back();
            binds.pop_back();

            users.updateBindings();
            GTEST_ASSERT_EQ(users._mouseButtonToAction.size(), 1);
            GTEST_ASSERT_EQ(users._keyToAction.size(), 1);
            GTEST_ASSERT_EQ(users._gamepadButtonToAction.size(), 1);
            GTEST_ASSERT_EQ(users._gamepadAxisToAction.size(), 0);
        }
    };
} // namespace ecstasy::integration::user_action

TEST(Users, removeOutdatedBindings)
{
    user_action::UsersTester::removeOutdatedBindings();
}

TEST(Users, addBindingIfMissing)
{
    user_action::UsersTester::addBindingIfMissing();
}

TEST(Users, updateBindings)
{
    user_action::UsersTester::updateBindings();
}

TEST(Users, updateBindingsStatic)
{
    ecstasy::Registry registry;

    /// Must not crash even if not present
    user_action::Users::updateBindings(registry);

    registry.addResource<user_action::Users>();
    user_action::Users::updateBindings(registry);
}

TEST(Users, handleEvent)
{
    ecstasy::Registry registry;
    user_action::Users &users = registry.addResource<user_action::Users>();

    auto &binds = users.getUserProfile().getActionBindings().getBindings();
    binds.emplace_back(0, event::Mouse::Button::Left);
    binds.emplace_back(1, event::Keyboard::Key::G);
    binds.emplace_back(2, event::Gamepad::Button::BumperLeft);
    binds.emplace_back(3, event::Gamepad::Axis::TriggerLeft);
    users.updateBindings();

    float values[5] = {0.f};

    /// Match any action
    registry.entityBuilder()
        .with<user_action::ActionListener>(
            [&values](ecstasy::Registry &r, ecstasy::Entity e, user_action::Action a) {
                (void)r;
                (void)e;
                values[0] += a.value + 1.f;
            },
            user_action::Action::All)
        .build();
    /// Match Action 0 (mouse)
    registry.entityBuilder()
        .with<user_action::ActionListener>(
            [&values](ecstasy::Registry &r, ecstasy::Entity e, user_action::Action a) {
                (void)r;
                (void)e;
                values[1] += a.value + 1.f;
            },
            0)
        .build();
    registry.entityBuilder()
        .with<user_action::ActionListeners>({/// Match action 1 (keyboard)
            {1,
                [&values](ecstasy::Registry &r, ecstasy::Entity e, user_action::Action a) {
                    (void)r;
                    (void)e;
                    values[2] += a.value + 1.f;
                }},
            /// Match action 2 (gamepad button)
            {2,
                [&values](ecstasy::Registry &r, ecstasy::Entity e, user_action::Action a) {
                    (void)r;
                    (void)e;
                    values[3] += a.value + 1.f;
                }},
            /// Match action 3 (gamepad axis)
            {3,
                [&values](ecstasy::Registry &r, ecstasy::Entity e, user_action::Action a) {
                    (void)r;
                    (void)e;
                    values[4] += a.value + 1.f;
                }}})
        .build();

    /// Event not handled by the Users::handleEvent
    event::EventsManager::handleEvent(registry, event::MouseMoveEvent(50, 40));
    GTEST_ASSERT_EQ(values[0], 0.f);
    /// Event handled but not matching any action
    event::EventsManager::handleEvent(registry, event::MouseButtonEvent(event::Mouse::Button::Right, false));
    GTEST_ASSERT_EQ(values[0], 0.f);

    /// Mouse button release and pressed event (value += 0 + 1, value += 1 + 1)
    event::EventsManager::handleEvent(registry, event::MouseButtonEvent(event::Mouse::Button::Left, false));
    GTEST_ASSERT_EQ(values[0], 1.f);
    GTEST_ASSERT_EQ(values[1], 1.f);
    event::EventsManager::handleEvent(registry, event::MouseButtonEvent(event::Mouse::Button::Left, true));
    GTEST_ASSERT_EQ(values[0], 3.f);
    GTEST_ASSERT_EQ(values[1], 3.f);

    /// Keyboard release and pressed event (value += 0 + 1, value += 1 + 1)
    event::EventsManager::handleEvent(registry, event::KeyEvent(event::Keyboard::Key::G, false));
    GTEST_ASSERT_EQ(values[0], 4.f);
    GTEST_ASSERT_EQ(values[2], 1.f);
    event::EventsManager::handleEvent(registry, event::KeyEvent(event::Keyboard::Key::G, true));
    GTEST_ASSERT_EQ(values[0], 6.f);
    GTEST_ASSERT_EQ(values[2], 3.f);

    /// Gamepad button release and pressed event (value += 0 + 1, value += 1 + 1)
    event::EventsManager::handleEvent(
        registry, event::GamepadButtonEvent(0, event::Gamepad::Button::BumperLeft, false));
    GTEST_ASSERT_EQ(values[0], 7.f);
    GTEST_ASSERT_EQ(values[3], 1.f);
    event::EventsManager::handleEvent(registry, event::GamepadButtonEvent(0, event::Gamepad::Button::BumperLeft, true));
    GTEST_ASSERT_EQ(values[0], 9.f);
    GTEST_ASSERT_EQ(values[3], 3.f);

    /// Gamepad axis release and pressed event (value += 0.5 + 1)
    event::EventsManager::handleEvent(registry, event::GamepadAxisEvent(0, event::Gamepad::Axis::TriggerLeft, 0.5f));
    GTEST_ASSERT_EQ(values[0], 10.5f);
    GTEST_ASSERT_EQ(values[4], 1.5f);
}
