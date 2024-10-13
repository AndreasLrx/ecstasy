///
/// @file GamepadSequenceListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-03-27
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_LISTENERS_GAMEPADSEQUENCELISTENER_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_LISTENERS_GAMEPADSEQUENCELISTENER_HPP_

#include <functional>
#include <vector>
#include "ecstasy/integrations/event/inputs/Gamepad.hpp"
#include "ecstasy/resources/entity/Entity.hpp"

namespace ecstasy
{
    class Registry;
}

namespace ecstasy::integration::event
{
    struct GamepadButtonEvent;

    ///
    /// @brief Listener of a button sequence. Button sequences of buttons A, B, X are triggered only when A, B and X are
    /// pressed and released one at a time (A, then B, then X). If any other button is pressed while waiting for a
    /// button release or if the button was not expected, the validated buttons are reset: A, B, Y, X will not work
    /// because the Y reset the sequence.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-03-27)
    ///
    class GamepadSequenceListener {
      public:
        /// @brief Callback type.
        using Callback = std::function<void(Registry &, Entity, const GamepadSequenceListener &)>;

        ///
        /// @brief Construct a new Gamepad Sequence Listener object.
        ///
        /// @warning An empty sequence will result in undefined behavior, and probably to a crash.
        /// @note A sequence containing only one button is just a button listener.
        ///
        /// @param[in] sequence Button sequence to listen to.
        /// @param[in] callback Callback called when the sequence is validated.
        /// @param[in] gamepadId Gamepad ID to listen to (default to 0).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        GamepadSequenceListener(
            const std::vector<Gamepad::Button> &sequence, Callback callback, size_t gamepadId = 0) noexcept;

        ///
        /// @brief Default destructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        ~GamepadSequenceListener() noexcept = default;

        ///
        /// @brief Update the sequence with the given @ref GamepadButtonEvent.
        ///
        /// @param[in] event Updating event.
        ///
        /// @return bool Whether the sequence was validated or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        bool update(const GamepadButtonEvent &event) noexcept;

        ///
        /// @brief Check whether the sequence is complete or not.
        ///
        /// @return bool Whether the sequence is complete or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        [[nodiscard]] bool isComplete() const noexcept;

        ///
        /// @brief Reset the sequence completion.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        constexpr void reset() noexcept
        {
            _validatedButtons.clear();
            _heldButton = Gamepad::Button::Unknown;
        }

        ///
        /// @brief Call the callback and @ref reset() if the sequence is complete or if @p force.
        ///
        /// @param[in] registry Registry to forward to the callback.
        /// @param[in] e Entity owning this component.
        /// @param[in] force Whether the callback must be called regardless of the sequence completion.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        void operator()(Registry &registry, Entity e, bool force = false);

        ///
        /// @brief Change the expected sequence.
        ///
        /// @warning This function reset the sequence completion.
        ///
        /// @param[in] newSequence New expected sequence.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        void setSequence(const std::vector<Gamepad::Button> &newSequence) noexcept;

        ///
        /// @brief Get the expected sequence.
        ///
        /// @return const std::vector<Gamepad::Button>& A const reference to the expected sequence.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        [[nodiscard]] constexpr const std::vector<Gamepad::Button> &getSequence() const noexcept
        {
            return _sequence;
        }

        ///
        /// @brief Get the expected sequence.
        ///
        /// @warning If the sequence is updated, you should call @ref reset().
        ///
        /// @return std::vector<Gamepad::Button>& A reference to the expected sequence.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        [[nodiscard]] constexpr std::vector<Gamepad::Button> &getSequence() noexcept
        {
            return _sequence;
        }

        ///
        /// @brief Get the sequence completion callback.
        ///
        /// @return const Callback& A reference to the sequence completion callback.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        [[nodiscard]] constexpr const Callback &getCallback() const noexcept
        {
            return _callback;
        }

        ///
        /// @brief Get the button held.
        ///
        /// @note The button held is the current sequence button pressed but not yet released (it will be pushed in the
        /// validated buttons on release)
        ///
        /// @return Gamepad::Button Button held if any, @ref Gamepad::Button::Unknown otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        [[nodiscard]] constexpr Gamepad::Button getHeldButton() const noexcept
        {
            return _heldButton;
        }

        ///
        /// @brief Get the Validated Buttons.
        ///
        /// @note Use this function to see the progress of the sequence.
        ///
        /// @return const std::vector<Gamepad::Button>& A const reference to the validated buttons vector.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        [[nodiscard]] constexpr const std::vector<Gamepad::Button> &getValidatedButtons() const noexcept
        {
            return _validatedButtons;
        }

        ///
        /// @brief Get the Gamepad Id
        ///
        /// @return constexpr size_t Associated gamepad ID
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-28)
        ///
        [[nodiscard]] constexpr size_t getGamepadId() const noexcept
        {
            return _gamepadId;
        }

        ///
        /// @brief Change the Gamepad Id
        ///
        /// @warning This function reset the combination completion.
        ///
        /// @param[in] gamepadId new gamepad id associated with this listener.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-28)
        ///
        constexpr void setGamepadId(size_t gamepadId) noexcept
        {
            _gamepadId = gamepadId;
            reset();
        }

      private:
        // Expected sequence of buttons to watch for.
        std::vector<Gamepad::Button> _sequence;
        // Validated buttons.
        std::vector<Gamepad::Button> _validatedButtons;
        // Button held (if any, otherwise @ref Gamepad::Button::Unknown).
        Gamepad::Button _heldButton;
        // Callback called when the sequence is validated.
        Callback _callback;
        // Gamepad id to watch for.
        size_t _gamepadId;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_LISTENERS_GAMEPADSEQUENCELISTENER_HPP_ */
