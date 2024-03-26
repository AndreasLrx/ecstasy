///
/// @file KeyCombinationListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-20
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_LISTENERS_KEYCOMBINATIONLISTENER_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_LISTENERS_KEYCOMBINATIONLISTENER_HPP_

#include <functional>
#include <vector>
#include "ecstasy/integrations/event/inputs/Keyboard.hpp"
#include "ecstasy/resources/entity/Entity.hpp"

namespace ecstasy
{
    class Registry;
}

namespace ecstasy::integration::event
{
    struct KeyEvent;

    ///
    /// @brief Listener of a key combination. Key combination of keys A, B, C are triggered only when A, B and C are
    /// pressed at the same time.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-20)
    ///
    class KeyCombinationListener {
      public:
        /// @brief Callback type.
        using Callback = std::function<void(Registry &, Entity, const KeyCombinationListener &)>;

        ///
        /// @brief Construct a new Key Combination Listener object.
        ///
        /// @warning An empty combination will result in undefined behavior, and probably to a crash.
        /// @note A combination containing only one key is just a key listener.
        ///
        /// @param[in] combination Key combination to listen to.
        /// @param[in] callback Callback called when the combination is validated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        KeyCombinationListener(const std::vector<Keyboard::Key> &combination, Callback callback);

        ///
        /// @brief Default destructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        ~KeyCombinationListener() = default;

        ///
        /// @brief Update the combination with the given @ref KeyEvent.
        ///
        /// @param[in] event Updating event.
        ///
        /// @return bool Whether the combination was validated or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        bool update(const KeyEvent &event);

        ///
        /// @brief Check whether the combination is complete or not.
        ///
        /// @return bool Whether the combination is complete or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        bool isComplete() const;

        ///
        /// @brief Reset the combination completion.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
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
        /// @since 1.0.0 (2022-11-20)
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
        /// @since 1.0.0 (2022-11-20)
        ///
        void setCombination(const std::vector<Keyboard::Key> &newCombination);

        ///
        /// @brief Get the expected combination.
        ///
        /// @return const std::vector<Keyboard::Key>& A const reference to the expected combination.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        constexpr const std::vector<Keyboard::Key> &getCombination() const
        {
            return _combination;
        }

        ///
        /// @brief Get the expected combination.
        ///
        /// @warning If the combination is updated, you should call @ref reset().
        ///
        /// @return std::vector<Keyboard::Key>& A reference to the expected combination.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        constexpr std::vector<Keyboard::Key> &getCombination()
        {
            return _combination;
        }

        ///
        /// @brief Get the combination completion callback.
        ///
        /// @return const Callback& A reference to the combination completion callback.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        constexpr const Callback &getCallback() const
        {
            return _callback;
        }

        ///
        /// @brief Get the count of validated keys
        ///
        /// @return size_t Count of validated keys.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        constexpr size_t getValidatedKeys() const
        {
            return _validatedKeys;
        }

        ///
        /// @brief Get the Key States.
        ///
        /// @note The map key correspond to a combination key and the value is whether it is pressed or not.
        ///
        /// @return const std::unordered_map<Keyboard::Key, bool>& A const reference to the KeyStates map.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        constexpr const std::unordered_map<Keyboard::Key, bool> &getKeyStates() const
        {
            return _keyStates;
        }

      private:
        std::vector<Keyboard::Key> _combination;
        std::unordered_map<Keyboard::Key, bool> _keyStates;
        size_t _validatedKeys;
        Callback _callback;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_LISTENERS_KEYCOMBINATIONLISTENER_HPP_ */
