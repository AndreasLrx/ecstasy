///
/// @file UserProfile.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
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

        /// @brief Constant id matching any user ID.
        static constexpr Id All = static_cast<Id>(-1);

        ///
        /// @brief Construct a new user profile with an associated id.
        ///
        /// @param[in] id User identifier.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        UserProfile(Id id = 0) noexcept : _id(id)
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
        [[nodiscard]] constexpr ActionBindings &getActionBindings() noexcept
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
        [[nodiscard]] constexpr const ActionBindings &getActionBindings() const noexcept
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
        [[nodiscard]] constexpr Id getId() const noexcept
        {
            return _id;
        }

        ///
        /// @brief Change the user ID.
        ///
        /// @param[in] id new id.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-05)
        ///
        constexpr void setId(Id id) noexcept
        {
            _id = id;
        }

        ///
        /// @brief Dump the user profile informations as a toml table.
        ///
        /// @return toml::table Toml representation of the user profile.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-29)
        ///
        [[nodiscard]] toml::table dump() const noexcept;

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
        bool load(const toml::table &infos) noexcept;

      private:
        // User identifier
        Id _id;
        // User action bindings
        ActionBindings _actionBindings;
    };
} // namespace ecstasy::integration::user_action

#endif /* !ECSTASY_INTEGRATIONS_USER_ACTION_USERPROFILE_HPP_ */
