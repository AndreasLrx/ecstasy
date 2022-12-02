///
/// @file UserProfile.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_USER_ACTION_USERPROFILE_HPP_
#define ECSTASY_INTEGRATIONS_USER_ACTION_USERPROFILE_HPP_

#include <toml++/toml.h>

#include "ActionBindings.hpp"

namespace ecstasy::integration::user_action
{
    ///
    /// @brief Represent an application user profile, allowing to configure basic informations like the action bindings.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-25)
    ///
    class UserProfile {
      public:
        /// @brief User identifier type
        using Id = size_t;

        ///
        /// @brief Construct a new user profile with an associated id.
        ///
        /// @param[in] id User identifier.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        UserProfile(Id id = 0) : _id(id)
        {
        }

        ///
        /// @brief Get the Action Bindings.
        ///
        /// @return ActionBinding& A reference to the ActionBindings object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        constexpr ActionBindings &getActionBindings()
        {
            return _actionBindings;
        }

        ///
        /// @brief Get the Action Bindings.
        ///
        /// @return const ActionBindings& A reference to the ActionBindings object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        constexpr const ActionBindings &getActionBindings() const
        {
            return _actionBindings;
        }

        ///
        /// @brief Get the Id of the profile.
        ///
        /// @note Starts at 0.
        ///
        /// @return Id Profile id.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-02)
        ///
        constexpr Id getId() const
        {
            return _id;
        }

        ///
        /// @brief Dump the user profile informations as a toml table.
        ///
        /// @return toml::table Toml representation of the user profile.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-29)
        ///
        toml::table dump() const;

        ///
        /// @brief Load a given toml table representing a user profile. See the format of @ref dump().
        ///
        /// @warning This method clear the internal data.
        ///
        /// @param[in] infos User profile dump.
        ///
        /// @return bool Whether the load succeed or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-30)
        ///
        bool load(const toml::table &infos);

      private:
        Id _id;
        ActionBindings _actionBindings;
    };
} // namespace ecstasy::integration::user_action

#endif /* !ECSTASY_INTEGRATIONS_USER_ACTION_USERPROFILE_HPP_ */
