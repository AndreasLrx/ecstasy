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

#ifndef USERPROFILE_HPP_
#define USERPROFILE_HPP_

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
        constexpr UserProfile(Id id = 0) : _id(id)
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

      private:
        Id _id;
        ActionBindings _actionBindings;
    };
} // namespace ecstasy::integration::user_action

#endif /* !USERPROFILE_HPP_ */
