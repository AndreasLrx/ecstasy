/*
** EPITECH PROJECT, 2022
** ecstasy
** File description:
** ModifiersList
*/

#ifndef ECSTASY_QUERY_MODIFIERSLIST_HPP_
#define ECSTASY_QUERY_MODIFIERSLIST_HPP_

#include <memory>
#include <vector>

#include "Modifier.hpp"

namespace ecstasy
{
    /// @internal
    /// @brief Wrapper of a @ref Modifier vector to manage lifetime of multiple modifiers.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    class ModifiersList {
      public:
        ///
        /// @brief Instanciate a new modifier with lifetime attached to @b this lifetime.
        ///
        /// @tparam M Type of the modifier.
        /// @tparam Args Arguments Types of the modifier constructor.
        ///
        /// @param[in] args Arguments to forward to the modifier constructor.
        ///
        /// @return M& newly created modifier.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        template <std::derived_from<Modifier> M, typename... Args>
        M &instanciateModifier(Args &&...args)
        {
            return dynamic_cast<M &>(*_modifiers.emplace_back(std::make_unique<M>(std::forward<Args>(args)...)).get());
        }

      private:
        std::vector<std::unique_ptr<Modifier>> _modifiers;
    };
} // namespace ecstasy

#endif /* !ECSTASY_QUERY_MODIFIERSLIST_HPP_ */
