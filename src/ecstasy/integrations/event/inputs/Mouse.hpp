///
/// @file Mouse.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-04
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATION_EVENT_INPUTS_MOUSE_HPP_
#define ECSTASY_INTEGRATION_EVENT_INPUTS_MOUSE_HPP_

#include <array>
#include <iostream>
#include <utility>

#include "ecstasy/resources/Resource.hpp"
#include "util/serialization/SerializableEnum.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Current mouse state.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-04)
    ///
    class Mouse : public Resource<Mouse> {
      public:
        // LCOV_EXCL_START

        SERIALIZABLE_ENUM(Button, 0, Left, Right, Middle, Extra1, Extra2, Extra3, Count)

        // LCOV_EXCL_STOP
#ifdef _DOXYGEN_
        /// @brief Mouse buttons
        enum class Button {
            Left,   ///< The left mouse button
            Right,  ///< The right mouse button
            Middle, ///< The middle (wheel) mouse button
            Extra1, ///< The first extra mouse button
            Extra2, ///< The second extra mouse button
            Extra3, ///< The third extra mouse button

            Count ///< Keep last -- the total number of mouse buttons
        };
#endif

        /// @brief Mouse wheels
        enum class Wheel {
            Horizontal, ///< Horizontal mouse wheel (common)
            Vertical,   ///< Vertical mouse wheel

            Count ///< Keep last -- the total number of mouse wheels
        };

        ///
        /// @brief Construct a new Mouse resource.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        Mouse() noexcept : _buttons({false}), _x(0), _y(0){};

        ///
        /// @brief Destroy the Mouse resource.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        virtual ~Mouse() noexcept = default;

        ///
        /// @brief Check whether a mouse button is down.
        ///
        /// @param[in] button evaluated button.
        ///
        /// @return bool Whether the button is down or not.
        ///
        /// @throw std::invalid_argument If the button is invalid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        [[nodiscard]] constexpr bool isButtonDown(Button button) const
        {
            assertButtonValid(button);
            return _buttons[static_cast<size_t>(button)];
        }

        ///
        /// @brief Check whether a mouse button is up.
        ///
        /// @param[in] button evaluated button.
        ///
        /// @return bool Whether the button is up or not.
        ///
        /// @throw std::invalid_argument If the button is invalid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        [[nodiscard]] constexpr bool isButtonUp(Button button) const
        {
            return !isButtonDown(button);
        }

        ///
        /// @brief Update a given button state.
        ///
        /// @warning It only update the internal state, no event is called.
        ///
        /// @param[in] button Button to update.
        /// @param[in] down Whether the button must be set down or not.
        ///
        /// @throw std::invalid_argument If the button is invalid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        constexpr void setButtonState(Button button, bool down)
        {
            assertButtonValid(button);
            _buttons[static_cast<size_t>(button)] = down;
        }

        ///
        /// @brief Get the X position of the mouse on the window.
        ///
        /// @return int x position.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        [[nodiscard]] constexpr int getX() const noexcept
        {
            return _x;
        }

        ///
        /// @brief Update the mouse x position.
        ///
        /// @warning It only update the internal state, no event is called.
        ///
        /// @param[in] x new x position.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        constexpr void setX(int x) noexcept
        {
            _x = x;
        }

        ///
        /// @brief Get the y position of the mouse on the window.
        ///
        /// @return int y position.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        [[nodiscard]] constexpr int getY() const noexcept
        {
            return _y;
        }

        ///
        /// @brief Update the mouse y position.
        ///
        /// @warning It only update the internal state, no event is called.
        ///
        /// @param[in] y new y position.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        constexpr void setY(int y) noexcept
        {
            _y = y;
        }

        ///
        /// @brief Get the mouse position on the window.
        ///
        /// @return @ref std::pair<int, int> mouse position.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        [[nodiscard]] constexpr std::pair<int, int> getPosition() noexcept
        {
            return std::make_pair(_x, _y);
        }

        ///
        /// @brief Set the mouse position.
        ///
        /// @warning It only update the internal state, no event is called.
        ///
        /// @param[in] x x position.
        /// @param[in] y y position.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-05)
        ///
        constexpr void setPosition(int x, int y) noexcept
        {
            _x = x;
            _y = y;
        }

        ///
        /// @brief Assert the validity of a mouse button.
        ///
        /// @param[in] button Button to assert.
        ///
        /// @throw std::invalid_argument If the button is invalid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-13)
        ///
        constexpr static void assertButtonValid(Button button)
        {
            if (static_cast<std::size_t>(button) >= static_cast<std::size_t>(Button::Count)) [[unlikely]]
                throw std::invalid_argument("Invalid mouse button");
        }

      private:
        // Mouse buttons state
        std::array<bool, static_cast<int>(Button::Count)> _buttons;
        // Mouse X position
        int _x;
        // Mouse Y position
        int _y;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENT_INPUTS_MOUSE_HPP_ */
