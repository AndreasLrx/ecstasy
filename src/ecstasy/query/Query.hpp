/*
** EPITECH PROJECT, 2022
** ecstasy
** File description:
** Query
*/

#ifndef ECSTASY_QUERY_QUERY_HPP_
#define ECSTASY_QUERY_QUERY_HPP_

#include "util/BitSet.hpp"

namespace ecstasy
{
    class Entities;
    class IStorage;

    class Query {
      public:
        ///
        /// @brief Construct a new Query initialized with the entities alive.
        ///
        /// @param[in] entities Entities manager.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        Query(Entities &entities);

        ///
        /// @brief Perform a AND request using the given component storage.
        ///
        /// @param[in] storage Component storage.
        ///
        /// @return Query& @b this.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        Query &where(IStorage &storage);

        ///
        /// @brief Get the Query Mask. All bit set means an entity match the chained request.
        ///
        /// @return const util::BitSet& Query BitMask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        constexpr const util::BitSet &getMask() const
        {
            return _mask;
        }

      private:
        util::BitSet _mask;
    };
} // namespace ecstasy

#endif /* !QUERY_HPP_ */
