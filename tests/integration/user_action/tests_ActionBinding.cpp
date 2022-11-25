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