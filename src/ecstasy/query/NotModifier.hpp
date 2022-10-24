///
/// @file NotModifier.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-24
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_NOTMODIFIER_HPP_
#define ECSTASY_QUERY_NOTMODIFIER_HPP_

#include "QueryConcepts.hpp"

namespace ecstasy
{
    ///
    /// @brief Query modifier which simply inverts the bits.
    ///
    /// @warning Since it returns all entities that @b haven't the requested data, a call to @ref getQueryData() is
    /// undefined behavior and will cause errors or exceptions depending on the Queryable implementation. Therefore you
    /// should @b never select this queryable.
    ///
    /// @tparam Internal Type of the wrapped queryable.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <Queryable Internal>
    class Not {
      public:
        /// @brief @ref Queryable constaint.
        using QueryData = Internal::QueryData;

        ///
        /// @brief Construct a new Not Queryable modifier.
        ///
        /// @param[in] internal wrapped queryable.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        Not(Internal &internal) : _internal(internal), _mask(internal.getMask())
        {
            _mask.flip();
        }

        ///
        /// @brief Get the flipped mask of the internal queryable.
        ///
        /// @note @ref Queryable constraint.
        ///
        /// @return const util::BitSet& Flipped bitset.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        const util::BitSet &getMask() const
        {
            return _mask;
        }

        ///
        /// @brief Get the internal query data at the given index.
        ///
        /// @note @ref Queryable constraint.
        /// @warning May throw exceptions, look at the @b Internal type equivalent function documentation.
        /// @warning Since this is a not modifier, all bit set to true in the masks means there @b isn't data at the
        /// associated index. Therefore you @b must @b not call this function for bits set to true in the mask.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return QueryData internal queryable data.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        QueryData getQueryData(size_t index)
        {
            return _internal.getQueryData(index);
        }

        ///
        /// @brief Update the mask to be up to date with the internal bitmask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        void reloadMask()
        {
            _mask = ~_internal.getMask();
        }

      private:
        Internal &_internal;
        util::BitSet _mask;
    };
} // namespace ecstasy

#endif /* !ECSTASY_QUERY_NOTMODIFIER_HPP_ */
