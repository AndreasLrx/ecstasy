///
/// @file ActionBindings.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONBINDINGS_HPP_
#define ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONBINDINGS_HPP_

#include <vector>
#include "ActionBinding.hpp"
#include <toml++/toml.h>

namespace ecstasy::integration::user_action
{
    ///
    /// @brief Wrapper of a @ref std::vector of ActionBinding.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-25)
    ///
    class ActionBindings {
      public:
        ///
        /// @brief Construct a new empty action binding container.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        ActionBindings() noexcept = default;

        ///
        /// @brief Get the Bindings.
        ///
        /// @return std::vector<ActionBinding>& A reference to the internal container.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        [[nodiscard]] constexpr std::vector<ActionBinding> &getBindings() noexcept
        {
            return _bindings;
        }

        ///
        /// @brief Get the Bindings.
        ///
        /// @return const std::vector<ActionBinding>& A const reference to the internal container.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-25)
        ///
        [[nodiscard]] constexpr const std::vector<ActionBinding> &getBindings() const noexcept
        {
            return _bindings;
        }

        ///
        /// @brief Dump the bindings as a toml table.
        ///
        /// @return toml::table Toml representation of the bindings.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-29)
        ///
        [[nodiscard]] toml::table dump() const noexcept;

        ///
        /// @brief Load a given toml table representing a list of bindings. See the format of @ref dump().
        ///
        /// @warning This method clear the internal bindings.
        ///
        /// @param[in] bindings Input bindings.
        ///
        /// @return bool Whether the load succeed or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-30)
        ///
        bool load(const toml::table &bindings) noexcept;

        ///
        /// @brief Check whether @p action is contained in the internal @ref ActionBinding vector.
        ///
        /// @param[in] action action to search.
        ///
        /// @return bool Whether the action is contained or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-02)
        ///
        [[nodiscard]] bool contains(ActionBinding action) const noexcept;

      private:
        // Internal bindings.
        std::vector<ActionBinding> _bindings;
    };
} // namespace ecstasy::integration::user_action

#endif /* !ECSTASY_INTEGRATIONS_USER_ACTION_ACTIONBINDINGS_HPP_ */
