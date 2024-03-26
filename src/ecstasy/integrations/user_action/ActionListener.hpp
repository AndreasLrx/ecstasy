///
/// @file ActionListener.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-02
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONLISTENER_HPP_
#define ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONLISTENER_HPP_

#include "ecstasy/integrations/event/listeners/EventListeners.hpp"
#include "ecstasy/integrations/user_action/UserProfile.hpp"
#include <unordered_map>

namespace ecstasy::integration::user_action
{
    ///
    /// @brief Action listener component. Contains a callback called when the associated action is detected.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-02)
    ///
    struct ActionListener {
        /// @brief Listener type.
        using Listener = std::function<void(Registry &, Entity, Action)>;

        /// @brief Id of the target action.
        ///
        /// @note @ref Action::All will match all actions.
        Action::Id actionId;
        /// @brief Listener called on action detection.
        Listener listener;

        ///
        /// @brief Construct a new Action Listener.
        ///
        /// @param[in] pListener Listener.
        /// @param[in] pActionId Target action.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-02)
        ///
        ActionListener(Listener pListener, Action::Id pActionId = Action::All)
            : actionId(pActionId), listener(pListener)
        {
        }
    };

    ///
    /// @brief Action listener component templated with the target action id.
    ///
    /// @tparam Target Target action id.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-07)
    ///
    template <Action::Id Target = Action::All>
    struct ActionIdListener : public ActionListener {
        ///
        /// @brief Construct a new Action Id Listener.
        ///
        /// @param[in] pListener Listener.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-07)
        ///
        ActionIdListener(Listener pListener) : ActionListener(pListener, Target)
        {
        }
    };

    ///
    /// @brief Action listeners map. Used to listen to more than one Action on a single entity.
    ///
    /// @note Doesn't handle @ref Action::All .
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2023-01-31)
    ///
    using ActionListeners = std::unordered_map<Action::Id, ActionListener::Listener>;
} // namespace ecstasy::integration::user_action

#endif /* !ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONLISTENER_HPP_ */
