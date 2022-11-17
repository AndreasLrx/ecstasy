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
#include <ostream>

#include "ecstasy/resources/Resource.hpp"

/// @internal
#define ECSTASY_KEY_NAME_CASE(key) \
    case Key::key: return #key

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
        /// @brief Keyboard keys.
        enum class Key {
            Unknown = -1, ///< Unhandled key
            A = 0,        ///< The A key
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
        /// @return constexpr bool Whether the key is down.
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
        /// @return constexpr bool Whether the key is up.
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

        ///
        /// @brief Get the name of a key.
        ///
        /// @warning If the key is Key::Count or isn't a valid Key, @ref nullptr is returned.
        ///
        /// @param[in] key Evaluated key.
        ///
        /// @return constexpr const char* Key name if valid, nullptr otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-17)
        ///
        constexpr static const char *getKeyName(Key key)
        {
            switch (key) {
                ECSTASY_KEY_NAME_CASE(Unknown);
                ECSTASY_KEY_NAME_CASE(A);
                ECSTASY_KEY_NAME_CASE(B);
                ECSTASY_KEY_NAME_CASE(C);
                ECSTASY_KEY_NAME_CASE(D);
                ECSTASY_KEY_NAME_CASE(E);
                ECSTASY_KEY_NAME_CASE(F);
                ECSTASY_KEY_NAME_CASE(G);
                ECSTASY_KEY_NAME_CASE(H);
                ECSTASY_KEY_NAME_CASE(I);
                ECSTASY_KEY_NAME_CASE(J);
                ECSTASY_KEY_NAME_CASE(K);
                ECSTASY_KEY_NAME_CASE(L);
                ECSTASY_KEY_NAME_CASE(M);
                ECSTASY_KEY_NAME_CASE(N);
                ECSTASY_KEY_NAME_CASE(O);
                ECSTASY_KEY_NAME_CASE(P);
                ECSTASY_KEY_NAME_CASE(Q);
                ECSTASY_KEY_NAME_CASE(R);
                ECSTASY_KEY_NAME_CASE(S);
                ECSTASY_KEY_NAME_CASE(T);
                ECSTASY_KEY_NAME_CASE(U);
                ECSTASY_KEY_NAME_CASE(V);
                ECSTASY_KEY_NAME_CASE(W);
                ECSTASY_KEY_NAME_CASE(X);
                ECSTASY_KEY_NAME_CASE(Y);
                ECSTASY_KEY_NAME_CASE(Z);
                ECSTASY_KEY_NAME_CASE(Num0);
                ECSTASY_KEY_NAME_CASE(Num1);
                ECSTASY_KEY_NAME_CASE(Num2);
                ECSTASY_KEY_NAME_CASE(Num3);
                ECSTASY_KEY_NAME_CASE(Num4);
                ECSTASY_KEY_NAME_CASE(Num5);
                ECSTASY_KEY_NAME_CASE(Num6);
                ECSTASY_KEY_NAME_CASE(Num7);
                ECSTASY_KEY_NAME_CASE(Num8);
                ECSTASY_KEY_NAME_CASE(Num9);
                ECSTASY_KEY_NAME_CASE(Escape);
                ECSTASY_KEY_NAME_CASE(LControl);
                ECSTASY_KEY_NAME_CASE(LShift);
                ECSTASY_KEY_NAME_CASE(LAlt);
                ECSTASY_KEY_NAME_CASE(LSystem);
                ECSTASY_KEY_NAME_CASE(RControl);
                ECSTASY_KEY_NAME_CASE(RShift);
                ECSTASY_KEY_NAME_CASE(RAlt);
                ECSTASY_KEY_NAME_CASE(RSystem);
                ECSTASY_KEY_NAME_CASE(Menu);
                ECSTASY_KEY_NAME_CASE(LBracket);
                ECSTASY_KEY_NAME_CASE(RBracket);
                ECSTASY_KEY_NAME_CASE(Semicolon);
                ECSTASY_KEY_NAME_CASE(Comma);
                ECSTASY_KEY_NAME_CASE(Period);
                ECSTASY_KEY_NAME_CASE(Quote);
                ECSTASY_KEY_NAME_CASE(Slash);
                ECSTASY_KEY_NAME_CASE(Backslash);
                ECSTASY_KEY_NAME_CASE(Tilde);
                ECSTASY_KEY_NAME_CASE(Equal);
                ECSTASY_KEY_NAME_CASE(Hyphen);
                ECSTASY_KEY_NAME_CASE(Space);
                ECSTASY_KEY_NAME_CASE(Enter);
                ECSTASY_KEY_NAME_CASE(Backspace);
                ECSTASY_KEY_NAME_CASE(Tab);
                ECSTASY_KEY_NAME_CASE(PageUp);
                ECSTASY_KEY_NAME_CASE(PageDown);
                ECSTASY_KEY_NAME_CASE(End);
                ECSTASY_KEY_NAME_CASE(Home);
                ECSTASY_KEY_NAME_CASE(Insert);
                ECSTASY_KEY_NAME_CASE(Delete);
                ECSTASY_KEY_NAME_CASE(Add);
                ECSTASY_KEY_NAME_CASE(Subtract);
                ECSTASY_KEY_NAME_CASE(Multiply);
                ECSTASY_KEY_NAME_CASE(Divide);
                ECSTASY_KEY_NAME_CASE(Left);
                ECSTASY_KEY_NAME_CASE(Right);
                ECSTASY_KEY_NAME_CASE(Up);
                ECSTASY_KEY_NAME_CASE(Down);
                ECSTASY_KEY_NAME_CASE(Numpad0);
                ECSTASY_KEY_NAME_CASE(Numpad1);
                ECSTASY_KEY_NAME_CASE(Numpad2);
                ECSTASY_KEY_NAME_CASE(Numpad3);
                ECSTASY_KEY_NAME_CASE(Numpad4);
                ECSTASY_KEY_NAME_CASE(Numpad5);
                ECSTASY_KEY_NAME_CASE(Numpad6);
                ECSTASY_KEY_NAME_CASE(Numpad7);
                ECSTASY_KEY_NAME_CASE(Numpad8);
                ECSTASY_KEY_NAME_CASE(Numpad9);
                ECSTASY_KEY_NAME_CASE(F1);
                ECSTASY_KEY_NAME_CASE(F2);
                ECSTASY_KEY_NAME_CASE(F3);
                ECSTASY_KEY_NAME_CASE(F4);
                ECSTASY_KEY_NAME_CASE(F5);
                ECSTASY_KEY_NAME_CASE(F6);
                ECSTASY_KEY_NAME_CASE(F7);
                ECSTASY_KEY_NAME_CASE(F8);
                ECSTASY_KEY_NAME_CASE(F9);
                ECSTASY_KEY_NAME_CASE(F10);
                ECSTASY_KEY_NAME_CASE(F11);
                ECSTASY_KEY_NAME_CASE(F12);
                ECSTASY_KEY_NAME_CASE(F13);
                ECSTASY_KEY_NAME_CASE(F14);
                ECSTASY_KEY_NAME_CASE(F15);
                ECSTASY_KEY_NAME_CASE(Pause);
                default: return nullptr;
            }
        }

      private:
        std::array<bool, static_cast<int>(Key::Count)> _keys;
    };
} // namespace ecstasy::integration::event

#undef ECSTASY_KEY_NAME_CASE

///
/// @brief Output stream operator overload to have human readable name for each key.
///
/// @param[in] stream output stream.
/// @param[in] key Key to write.
///
/// @return std::ostream& @p stream.
///
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @since 1.0.0 (2022-11-17)
///
std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::event::Keyboard::Key &key);

#endif /* !ECSTASY_INTEGRATIONS_EVENT_INPUTS_KEYBOARD_HPP_ */
