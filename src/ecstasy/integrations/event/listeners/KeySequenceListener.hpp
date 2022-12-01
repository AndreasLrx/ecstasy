///
/// @file KeySequenceListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-20
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_LISTENERS_KEYSEQUENCELISTENER_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_LISTENERS_KEYSEQUENCELISTENER_HPP_

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
    /// @brief Listener of a key sequence. Key sequences of keys A, B, C are triggered only when A, B and C are pressed
    /// and released one at a time (A, then B, then C). If any other key is pressed while waiting for a key release or
    /// if the key was not expected, the validated keys are reset: A, B, Z, E will not work because the Z reset the
    /// sequence.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-20)
    ///
    class KeySequenceListener {
      public:
        /// @brief Callback type.
        using Callback = std::function<void(Registry &, Entity, const KeySequenceListener &)>;

        ///
        /// @brief Construct a new Key Sequence Listener object.
        ///
        /// @warning An empty sequence will result in undefined behavior, and probably to a crash.
        /// @note A sequence containing only one key is just a key listener.
        ///
        /// @param[in] sequence Key sequence to listen to.
        /// @param[in] callback Callback called when the sequence is validated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        KeySequenceListener(const std::vector<Keyboard::Key> &sequence, Callback callback);

        ///
        /// @brief Default destructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        ~KeySequenceListener() = default;

        ///
        /// @brief Update the sequence with the given @ref KeyEvent.
        ///
        /// @param[in] event Updating event.
        ///
        /// @return bool Whether the sequence was validated or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        bool update(const KeyEvent &event);

        ///
        /// @brief Check whether the sequence is complete or not.
        ///
        /// @return bool Whether the sequence is complete or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        bool isComplete() const;

        ///
        /// @brief Reset the sequence completion.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        void reset();

        ///
        /// @brief Call the callback and @ref reset() if the sequence is complete or if @p force.
        ///
        /// @param[in] registry Registry to forward to the callback.
        /// @param[in] e Entity owning this component.
        /// @param[in] force Whether the callback must be called regardless of the sequence completion.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
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
        /// @since 1.0.0 (2022-11-20)
        ///
        void setSequence(const std::vector<Keyboard::Key> &newSequence);

        ///
        /// @brief Get the expected sequence.
        ///
        /// @return const std::vector<Keyboard::Key>& A const reference to the expected sequence.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        constexpr const std::vector<Keyboard::Key> &getSequence() const
        {
            return _sequence;
        }

        ///
        /// @brief Get the expected sequence.
        ///
        /// @warning If the sequence is updated, you should call @ref reset().
        ///
        /// @return std::vector<Keyboard::Key>& A reference to the expected sequence.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        constexpr std::vector<Keyboard::Key> &getSequence()
        {
            return _sequence;
        }

        ///
        /// @brief Get the sequence completion callback.
        ///
        /// @return const Callback& A reference to the sequence completion callback.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        constexpr const Callback &getCallback() const
        {
            return _callback;
        }

        ///
        /// @brief Get the key held.
        ///
        /// @note The key held is the current sequence key pressed but not yet released (it will be pushed in the
        /// validated keys on release)
        ///
        /// @return Keyboard::Key Key held if any, @ref Keyboard::Key::Unknown otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        constexpr Keyboard::Key getHeldKey() const
        {
            return _heldKey;
        }

        ///
        /// @brief Get the Validated Keys.
        ///
        /// @note Use this function to see the progress of the sequence.
        ///
        /// @return const std::vector<Keyboard::Key>& A const reference to the validated keys vector.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-20)
        ///
        constexpr const std::vector<Keyboard::Key> &getValidatedKeys() const
        {
            return _validatedKeys;
        }

      private:
        std::vector<Keyboard::Key> _sequence;
        std::vector<Keyboard::Key> _validatedKeys;
        Keyboard::Key _heldKey;
        Callback _callback;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_LISTENERS_KEYSEQUENCELISTENER_HPP_ */
