///
/// @file ActionBinding.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONBINDING_HPP_
#define ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONBINDING_HPP_

#include <iostream>
#include <toml++/toml.h>
#include <unordered_map>

#include "Action.hpp"
#include "ecstasy/integrations/event/inputs/Gamepad.hpp"
#include "ecstasy/integrations/event/inputs/Keyboard.hpp"
#include "ecstasy/integrations/event/inputs/Mouse.hpp"
#include "util/serialization/SerializableEnum.hpp"

namespace ecstasy::integration::user_action
{
    /// @brief Anonymous namespace to simplify references to the ecstasy event integration namespace.
    namespace
    {
        namespace event = ecstasy::integration::event;
    }

    ///
    /// @brief Action binding class, represent a binding between an input and a given action.
    /// Holds the binding type (set to Type::Count if empty), the associated data type and the action id.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-25)
    ///
    class ActionBinding {
      public:
        // LCOV_EXCL_START

        SERIALIZABLE_ENUM(Type, MouseButton, Key, GamepadButton, GamepadAxis, Count)

        // LCOV_EXCL_STOP
#ifdef _DOXYGEN_
        /// @brief Binding types.
        enum class Type {
            MouseButton,   ///< The action is bound to a mouse button state.
            Key,           ///< The action is bound to a keyboard key state.
            GamepadButton, ///< The action is bound to a gamepad button state.
            GamepadAxis,   ///< The action is bound to a gamepad axis state.

            Count ///< Keep last - the total number of bindings.
        };
#endif

        ///
        /// @brief Construct an empty action binding.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        constexpr ActionBinding() : type(Type::Count), actionId(0)
        {
        }

        ///
        /// @brief Construct a mouse button action binding.
        ///
        /// @param[in] id Id of the associated action.
        /// @param[in] input Source input.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        constexpr ActionBinding(Action::Id id, event::Mouse::Button input)
            : type(Type::MouseButton), actionId(id), mouseButton(input)
        {
        }

        ///
        /// @brief Construct a key action binding.
        ///
        /// @param[in] id Id of the associated action.
        /// @param[in] input Source input.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        constexpr ActionBinding(Action::Id id, event::Keyboard::Key input) : type(Type::Key), actionId(id), key(input)
        {
        }

        ///
        /// @brief Construct a gamepad button action binding.
        ///
        /// @param[in] id Id of the associated action.
        /// @param[in] input Source input.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        constexpr ActionBinding(Action::Id id, event::Gamepad::Button input)
            : type(Type::GamepadButton), actionId(id), gamepadButton(input)
        {
        }

        ///
        /// @brief Construct a gamepad axis action binding.
        ///
        /// @param[in] id Id of the associated action.
        /// @param[in] input Source input.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        constexpr ActionBinding(Action::Id id, event::Gamepad::Axis input)
            : type(Type::GamepadAxis), actionId(id), gamepadAxis(input)
        {
        }

        ///
        /// @brief Equality operator overload.
        ///
        /// @param[in] other Other binding.
        ///
        /// @return bool Whether the two bindings are equal.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-02)
        ///
        constexpr bool operator==(const ActionBinding &other) const
        {
            if (type != other.type || actionId != other.actionId)
                return false;

            switch (type) {
                case Type::MouseButton: return mouseButton == other.mouseButton;
                case Type::Key: return key == other.key;
                case Type::GamepadButton: return gamepadButton == other.gamepadButton;
                case Type::GamepadAxis: return gamepadAxis == other.gamepadAxis;
                default: return true;
            }
        }

        ///
        /// @brief Type of the binding stored.
        ///
        /// @note @ref Type::Count means the binding is invalid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        Type type;

        ///
        /// @brief Associated action id.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        Action::Id actionId;

        ///
        /// @brief Anonymous union to ease facilitate the wrapped data access.
        ///
        /// @warning Only the member associated to the current @ref type is valid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        union {
            event::Mouse::Button mouseButton;     ///< @ref Type::MouseButton associated data.
            event::Keyboard::Key key;             ///< @ref Type::Key associated data.
            event::Gamepad::Button gamepadButton; ///< @ref Type::GamepadButton associated data.
            event::Gamepad::Axis gamepadAxis;     ///< @ref Type::GamepadAxis associated data.
        };
    };
} // namespace ecstasy::integration::user_action

#endif /* !ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONBINDING_HPP_ */
