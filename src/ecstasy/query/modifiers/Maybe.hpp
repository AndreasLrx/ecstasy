///
/// @file Maybe.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-24
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_MODIFIER_MAYBE_HPP_
#define ECSTASY_QUERY_MODIFIER_MAYBE_HPP_

#include <optional>
#include <type_traits>

#include "Modifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "util/BitSet.hpp"
#include "util/meta/add_optional.hpp"

namespace ecstasy::query::modifier
{
    ///
    /// @brief Query modifier which returns a std::optional filled when the data if existing.
    ///
    /// @note All bits are set to true.
    ///
    /// @tparam Internal Type of the wrapped queryable.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <Queryable Q>
    class Maybe : public Modifier {
      public:
        /// @brief Wrapped queryable.
        using Internal = Q;

        /// @brief @ref Modifier constraint.
        using Operands = std::tuple<Q>;

        /// @brief @ref ecstasy::query::Queryable constaint.
        using QueryData = util::meta::add_optional_t<typename Internal::QueryData>;

        ///
        /// @brief Construct a new Maybe Queryable modifier.
        ///
        /// @param[in] internal wrapped queryable.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        Maybe(Internal &internal) : _internal(internal)
        {
            adjustMask(internal.getMask().size());
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
        /// @brief Get a std::optional filled with the data at index @p index if existing.
        ///
        /// @note @ref ecstasy::query::Queryable constraint.
        /// @warning May throw exceptions, look at the @b Internal type equivalent method documentation.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return @ref QueryData A std::optional filled with the data at index @p index if existing.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        constexpr QueryData getQueryData(size_t index)
        {
            if (index < _internal.getMask().size() && _internal.getMask()[index])
                return QueryData{_internal.getQueryData(index)};
            return std::nullopt;
        }

        ///
        /// @brief Adjust the mask to be at least the @p maxSize.
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
            }
        }

      private:
        Internal &_internal;
        util::BitSet _mask;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIER_MAYBE_HPP_ */
