#include <gtest/gtest.h>
#include "ecstasy/integrations/user_action/ActionBinding.hpp"

namespace event = ecstasy::integration::event;
namespace user_action = ecstasy::integration::user_action;

TEST(ActionBinding, valid_types)
{
    /// Mouse button
    {
        user_action::ActionBinding binding(0, event::Mouse::Button::Left);

        GTEST_ASSERT_EQ(binding.type, user_action::ActionBinding::Type::MouseButton);
    }

    /// Keyboard key
    {
        user_action::ActionBinding binding(0, event::Keyboard::Key::A);

        GTEST_ASSERT_EQ(binding.type, user_action::ActionBinding::Type::Key);
    }

    /// Gamepad button
    {
        user_action::ActionBinding binding(0, event::Gamepad::Button::FaceRight);

        GTEST_ASSERT_EQ(binding.type, user_action::ActionBinding::Type::GamepadButton);
    }

    /// Gamepad axis
    {
        user_action::ActionBinding binding(0, event::Gamepad::Axis::LeftX);

        GTEST_ASSERT_EQ(binding.type, user_action::ActionBinding::Type::GamepadAxis);
    }
}

TEST(ActionBinding, equality)
{
    // Mouse
    user_action::ActionBinding mouseLeft(0, event::Mouse::Button::Left);
    user_action::ActionBinding mouseLeft1(1, event::Mouse::Button::Left);
    user_action::ActionBinding mouseRight(0, event::Mouse::Button::Right);
    // Key
    user_action::ActionBinding keyA(0, event::Keyboard::Key::A);
    user_action::ActionBinding keyB(0, event::Keyboard::Key::B);
    // Gamepad button
    user_action::ActionBinding gamepadRightB(0, event::Gamepad::Button::FaceRight);
    user_action::ActionBinding gamepadDownB(0, event::Gamepad::Button::FaceDown);
    // Gamepad axis
    user_action::ActionBinding gamepadLeftA(0, event::Gamepad::Axis::LeftX);
    user_action::ActionBinding gamepadRightA(0, event::Gamepad::Axis::RightY);

    user_action::ActionBinding empty;

    /// Different type or action id
    GTEST_ASSERT_NE(mouseLeft, keyA);
    GTEST_ASSERT_NE(mouseLeft, mouseLeft1);

    // Mouse equal and different
    GTEST_ASSERT_EQ(mouseLeft, mouseLeft);
    GTEST_ASSERT_NE(mouseLeft, mouseRight);

    // Key equal and different
    GTEST_ASSERT_EQ(keyA, keyA);
    GTEST_ASSERT_NE(keyA, keyB);

    // Gamepad button equal and different
    GTEST_ASSERT_EQ(gamepadRightB, gamepadRightB);
    GTEST_ASSERT_NE(gamepadRightB, gamepadDownB);

    // Gamepad axis equal and different
    GTEST_ASSERT_EQ(gamepadLeftA, gamepadLeftA);
    GTEST_ASSERT_NE(gamepadLeftA, gamepadRightA);

    // Type == Count -> only type and action id are check
    GTEST_ASSERT_EQ(empty, empty);
}