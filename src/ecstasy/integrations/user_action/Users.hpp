///
/// @file Users.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-02
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTAS_INTEGRATIONS_USER_ACTION_USERS_HPP_
#define ECSTAS_INTEGRATIONS_USER_ACTION_USERS_HPP_

#include <vector>
#include "UserProfile.hpp"
#include "ecstasy/integrations/event/events/Event.hpp"
#include "ecstasy/integrations/event/inputs/Gamepad.hpp"
#include "ecstasy/integrations/event/inputs/Keyboard.hpp"
#include "ecstasy/integrations/event/inputs/Mouse.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/resources/Resource.hpp"
#include "ecstasy/storages/MapStorage.hpp"
#include <unordered_map>

namespace
{
    namespace event = ecstasy::integration::event;
}

namespace ecstasy::integration::user_action
{
    ///
    /// @brief @ref UserProfile container resource. Manage all the user profiles and their associated actions and
    /// bindings.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-02)
    ///
    class Users : public ecstasy::Resource {
      private:
        /// @brief Helper type instead of using @ref std::pair.
        /// Describe a link between an action and a user.
        struct UserActionLink {
            /// @brief Action id.
            Action::Id actionId;
            /// @brief User id.
            UserProfile::Id userId;
        };

      public:
        ///
        /// @brief Construct a new Users resource.
        ///
        /// @param[in] count Number of available users.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-02)
        ///
        Users(size_t count = 1);

        ///
        /// @brief Default destructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-02)
        ///
        ~Users() = default;

        ///
        /// @brief Update the internal bindings associative maps.
        ///
        /// @note This must be called when one UserProfile binding changed.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-02)
        ///
        void updateBindings();

        ///
        /// @brief Fetch the @ref Users resource in the registry if available and call @ref updateBinding().
        ///
        /// @param[in] registry Registry owning the @ref Users resource.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-02)
        ///
        static void updateBindings(Registry &registry);

        ///
        /// @brief Call action listeners associated to the event if any.
        ///
        /// @param[in] registry Registry owning the event.
        /// @param[in] event Event to handle.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-02)
        ///
        void handleEvent(Registry &registry, const event::Event &event) const;

        ///
        /// @brief Get the User Profile corresponding to the given @p index.
        ///
        /// @warning This method does not perfom bounds checking.
        ///
        /// @param[in] index User index.
        ///
        /// @return UserProfile& A reference to the requested user.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-05)
        ///
        constexpr UserProfile &getUserProfile(size_t index = 0)
        {
            return _users[index];
        }

        /// @copydoc getUserProfile
        constexpr const UserProfile &getUserProfile(size_t index = 0) const
        {
            return _users[index];
        }

      private:
        /// @internal
        /// @brief Remove the bindings present in @p map and not in the @p _users bindings.
        template <typename T>
        void removeOutdatedBindings(std::unordered_multimap<T, UserActionLink> &map)
        {
            for (auto it = map.begin(); it != map.end();) {
                if (it->second.userId >= _users.size()
                    || !_users[it->second.userId].getActionBindings().contains(
                        ActionBinding(it->second.actionId, it->first)))
                    it = map.erase(it);
                else
                    ++it;
            }
        }

        /// @internal
        /// @brief Add the @p binding in the @p map if it is missing for the user @p user.
        template <typename T>
        void addBindingIfMissing(const UserProfile &user, const ActionBinding &binding, T input,
            std::unordered_multimap<T, UserActionLink> &map)
        {
            bool add = true;

            for (auto it = map.find(input); it != map.end(); ++it) {
                if (it->second.userId == user.getId() && it->second.actionId == binding.actionId) {
                    add = false;
                    break;
                }
            }
            if (add)
                map.insert({input, UserActionLink{binding.actionId, user.getId()}});
        }

        std::unordered_multimap<event::Mouse::Button, UserActionLink> _mouseButtonToAction;
        std::unordered_multimap<event::Keyboard::Key, UserActionLink> _keyToAction;
        std::unordered_multimap<event::Gamepad::Button, UserActionLink> _gamepadButtonToAction;
        std::unordered_multimap<event::Gamepad::Axis, UserActionLink> _gamepadAxisToAction;

        std::vector<UserProfile> _users;

        friend class UsersTester;
    };
} // namespace ecstasy::integration::user_action

#endif /* !ECSTAS_INTEGRATIONS_USER_ACTION_USERS_HPP_ */
