///
/// @file Not.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-24
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_MODIFIER_NOT_HPP_
#define ECSTASY_QUERY_MODIFIER_NOT_HPP_

#include "Modifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "util/BitSet.hpp"

namespace ecstasy::query::modifier
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
    template <Queryable Q>
    class Not : public Modifier {
      public:
        /// @brief Wrapped queryable.
        using Internal = Q;

        /// @brief @ref Modifier constraint.
        using Operands = std::tuple<Q>;

        /// @brief @ref ecstasy::query::Queryable constaint.
        using QueryData = typename Internal::QueryData;

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
        /// @note @ref ecstasy::query::Queryable constraint.
        ///
        /// @return const util::BitSet& Flipped bitset.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        constexpr const util::BitSet &getMask() const
        {
            return _mask;
        }

        ///
        /// @brief Get the internal query data at the given index.
        ///
        /// @note @ref ecstasy::query::Queryable constraint.
        /// @warning May throw exceptions, look at the @b Internal type equivalent method documentation.
        /// @warning Since this is a not modifier, all bit set to true in the masks means there @b isn't data at the
        /// associated index. Therefore you @b must @b not call this method for bits set to true in the mask.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return @ref QueryData internal queryable data.
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

        ///
        /// @brief Adjust the mask to be at least the @p maxSize.
        ///
        /// @note All bits exceeding the internal queryable mask size are set to 1.
        ///
        /// @param[in] maxSize maximum size of the masks compared with it.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-25)
        ///
        constexpr void adjustMask(size_t maxSize)
        {
            if (maxSize > _mask.size()) {
                _mask.resize(maxSize);
                _mask.setAll();
                _mask.inplaceXor(_internal.getMask());
            }
        }

      private:
        Internal &_internal;
        util::BitSet _mask;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIER_NOT_HPP_ */
