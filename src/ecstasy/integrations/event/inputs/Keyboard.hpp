///
/// @file Keyboard.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-16
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_INPUTS_KEYBOARD_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_INPUTS_KEYBOARD_HPP_

#include <array>
#include <iostream>
#include <unordered_map>

#include "ecstasy/resources/Resource.hpp"
#include "util/serialization/SerializableEnum.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Current Keyboard state.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-16)
    ///
    class Keyboard : public Resource {
      public:
        // LCOV_EXCL_START

        SERIALIZABLE_ENUM(Key, -1, Unknown, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y,
            Z, Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Escape, LControl, LShift, LAlt, LSystem,
            RControl, RShift, RAlt, RSystem, Menu, LBracket, RBracket, Semicolon, Comma, Period, Quote, Slash,
            Backslash, Tilde, Equal, Hyphen, Space, Enter, Backspace, Tab, PageUp, PageDown, End, Home, Insert, Delete,
            Add, Subtract, Multiply, Divide, Left, Right, Up, Down, Numpad0, Numpad1, Numpad2, Numpad3, Numpad4,
            Numpad5, Numpad6, Numpad7, Numpad8, Numpad9, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14,
            F15, Pause, Count)

        // LCOV_EXCL_STOP
#ifdef _DOXYGEN
        /// @brief Keyboard keys.
        enum class Key {
            Unknown = -1, ///< Unhandled key
            A,            ///< The A key
            B,            ///< The B key
            C,            ///< The C key
            D,            ///< The D key
            E,            ///< The E key
            F,            ///< The F key
            G,            ///< The G key
            H,            ///< The H key
            I,            ///< The I key
            J,            ///< The J key
            K,            ///< The K key
            L,            ///< The L key
            M,            ///< The M key
            N,            ///< The N key
            O,            ///< The O key
            P,            ///< The P key
            Q,            ///< The Q key
            R,            ///< The R key
            S,            ///< The S key
            T,            ///< The T key
            U,            ///< The U key
            V,            ///< The V key
            W,            ///< The W key
            X,            ///< The X key
            Y,            ///< The Y key
            Z,            ///< The Z key
            Num0,         ///< The 0 key
            Num1,         ///< The 1 key
            Num2,         ///< The 2 key
            Num3,         ///< The 3 key
            Num4,         ///< The 4 key
            Num5,         ///< The 5 key
            Num6,         ///< The 6 key
            Num7,         ///< The 7 key
            Num8,         ///< The 8 key
            Num9,         ///< The 9 key
            Escape,       ///< The Escape key
            LControl,     ///< The left Control key
            LShift,       ///< The left Shift key
            LAlt,         ///< The left Alt key
            LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
            RControl,     ///< The right Control key
            RShift,       ///< The right Shift key
            RAlt,         ///< The right Alt key
            RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
            Menu,         ///< The Menu key
            LBracket,     ///< The [ key
            RBracket,     ///< The ] key
            Semicolon,    ///< The ; key
            Comma,        ///< The , key
            Period,       ///< The . key
            Quote,        ///< The ' key
            Slash,        ///< The / key
            Backslash,    ///< The \ key
            Tilde,        ///< The ~ key
            Equal,        ///< The = key
            Hyphen,       ///< The - key (hyphen)
            Space,        ///< The Space key
            Enter,        ///< The Enter/Return keys
            Backspace,    ///< The Backspace key
            Tab,          ///< The Tabulation key
            PageUp,       ///< The Page up key
            PageDown,     ///< The Page down key
            End,          ///< The End key
            Home,         ///< The Home key
            Insert,       ///< The Insert key
            Delete,       ///< The Delete key
            Add,          ///< The + key
            Subtract,     ///< The - key (minus, usually from numpad)
            Multiply,     ///< The * key
            Divide,       ///< The / key
            Left,         ///< Left arrow
            Right,        ///< Right arrow
            Up,           ///< Up arrow
            Down,         ///< Down arrow
            Numpad0,      ///< The numpad 0 key
            Numpad1,      ///< The numpad 1 key
            Numpad2,      ///< The numpad 2 key
            Numpad3,      ///< The numpad 3 key
            Numpad4,      ///< The numpad 4 key
            Numpad5,      ///< The numpad 5 key
            Numpad6,      ///< The numpad 6 key
            Numpad7,      ///< The numpad 7 key
            Numpad8,      ///< The numpad 8 key
            Numpad9,      ///< The numpad 9 key
            F1,           ///< The F1 key
            F2,           ///< The F2 key
            F3,           ///< The F3 key
            F4,           ///< The F4 key
            F5,           ///< The F5 key
            F6,           ///< The F6 key
            F7,           ///< The F7 key
            F8,           ///< The F8 key
            F9,           ///< The F9 key
            F10,          ///< The F10 key
            F11,          ///< The F11 key
            F12,          ///< The F12 key
            F13,          ///< The F13 key
            F14,          ///< The F14 key
            F15,          ///< The F15 key
            Pause,        ///< The Pause key

            Count, ///< Keep last -- the total number of keyboard keys
        };
#endif

        ///
        /// @brief Construct a new Keyboard resource.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        constexpr Keyboard() : _keys({false}){};

        ///
        /// @brief Destroy the Keyboard.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        virtual ~Keyboard() = default;

        ///
        /// @brief Check whether a key is down.
        ///
        /// @param[in] key evaluated key.
        ///
        /// @return bool Whether the key is down.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        constexpr bool isKeyDown(Key key) const
        {
            return _keys[static_cast<size_t>(key)];
        }

        ///
        /// @brief Check whether a key is up.
        ///
        /// @param[in] key evaluated key.
        ///
        /// @return bool Whether the key is up.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        constexpr bool isKeyUp(Key key) const
        {
            return !isKeyDown(key);
        }

        ///
        /// @brief Update a given key state.
        ///
        /// @warning It only update the internal state, no event is called.
        ///
        /// @param[in] key Key to update.
        /// @param[in] down Whether the key must be set down or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        constexpr void setKeyState(Key key, bool down)
        {
            _keys[static_cast<size_t>(key)] = down;
        }

      private:
        std::array<bool, static_cast<int>(Key::Count)> _keys;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_INPUTS_KEYBOARD_HPP_ */
