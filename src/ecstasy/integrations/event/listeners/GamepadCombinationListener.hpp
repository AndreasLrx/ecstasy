///
/// @file GamepadCombinationListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-03-27
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_LISTENERS_GAMEPADCOMBINATIONLISTENER_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_LISTENERS_GAMEPADCOMBINATIONLISTENER_HPP_

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
    /// @brief Listener of a button combination. Gamepad combination of buttons A, B, X are triggered only when A, B and
    /// X are pressed at the same time.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-03-27)
    ///
    class GamepadCombinationListener {
      public:
        /// @brief Callback type.
        using Callback = std::function<void(Registry &, Entity, const GamepadCombinationListener &)>;

        ///
        /// @brief Construct a new Button Combination Listener object.
        ///
        /// @warning An empty combination will result in undefined behavior, and probably to a crash.
        /// @note A combination containing only one button is just a button listener.
        ///
        /// @param[in] combination Button combination to listen to.
        /// @param[in] callback Callback called when the combination is validated.
        /// @param[in] gamepadId Gamepad ID to listen to (default to 0).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        GamepadCombinationListener(
            const std::vector<Gamepad::Button> &combination, Callback callback, size_t gamepadId = 0);

        ///
        /// @brief Default destructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        ~GamepadCombinationListener() = default;

        ///
        /// @brief Update the combination with the given @ref GamepadButtonEvent.
        ///
        /// @param[in] event Updating event.
        ///
        /// @return bool Whether the combination was validated or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        bool update(const GamepadButtonEvent &event);

        ///
        /// @brief Check whether the combination is complete or not.
        ///
        /// @return bool Whether the combination is complete or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        bool isComplete() const;

        ///
        /// @brief Reset the combination completion.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        void reset();

        ///
        /// @brief Call the callback and @ref reset() if the combination is complete or if @p force.
        ///
        /// @param[in] registry Registry to forward to the callback.
        /// @param[in] e Entity owning this component.
        /// @param[in] force Whether the callback must be called regardless of the combination completion.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        void operator()(Registry &registry, Entity e, bool force = false);

        ///
        /// @brief Change the expected combination.
        ///
        /// @warning This function reset the combination completion.
        ///
        /// @param[in] newCombination New expected combination.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        void setCombination(const std::vector<Gamepad::Button> &newCombination);

        ///
        /// @brief Get the expected combination.
        ///
        /// @return const std::vector<Gamepad::Button>& A const reference to the expected combination.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        constexpr const std::vector<Gamepad::Button> &getCombination() const
        {
            return _combination;
        }

        ///
        /// @brief Get the expected combination.
        ///
        /// @warning If the combination is updated, you should call @ref reset().
        ///
        /// @return std::vector<Gamepad::Button>& A reference to the expected combination.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        constexpr std::vector<Gamepad::Button> &getCombination()
        {
            return _combination;
        }

        ///
        /// @brief Get the combination completion callback.
        ///
        /// @return const Callback& A reference to the combination completion callback.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        constexpr const Callback &getCallback() const
        {
            return _callback;
        }

        ///
        /// @brief Get the count of validated buttons
        ///
        /// @return size_t Count of validated buttons.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        constexpr size_t getValidatedButtons() const
        {
            return _validatedButtons;
        }

        ///
        /// @brief Get the Button States.
        ///
        /// @note The map button correspond to a combination button and the value is whether it is pressed or not.
        ///
        /// @return const std::unordered_map<Gamepad::Button, bool>& A const reference to the ButtonStates map.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-27)
        ///
        constexpr const std::unordered_map<Gamepad::Button, bool> &getButtonStates() const
        {
            return _buttonStates;
        }

        ///
        /// @brief Get the Gamepad Id
        ///
        /// @return constexpr size_t Associated gamepad ID
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-03-28)
        ///
        constexpr size_t getGamepadId() const
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
        void setGamepadId(size_t gamepadId)
        {
            _gamepadId = gamepadId;
            reset();
        }

      private:
        std::vector<Gamepad::Button> _combination;
        std::unordered_map<Gamepad::Button, bool> _buttonStates;
        size_t _validatedButtons;
        Callback _callback;
        size_t _gamepadId;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_LISTENERS_GAMEPADCOMBINATIONLISTENER_HPP_ */
