///
/// @file Maybe.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Query modifier which returns a std::optional filled when the data if existing.
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
#include "util/meta/Traits.hpp"
#include "util/meta/add_optional.hpp"

namespace ecstasy::query::modifier
{
    ///
    /// @brief Query modifier which returns a std::optional filled when the data if existing.
    ///
    /// @note All bits are set to true.
    ///
    /// @tparam Internal Type of the wrapped queryable.
    /// @tparam AutoLock Lock the @ref ecstasy::thread::Lockable "Lockable" queryables if true.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <Queryable Q, bool AutoLock = false>
    class Maybe : public Modifier<AutoLock> {
      public:
        /// @brief Wrapped queryable.
        using Internal = queryable_qualifiers_t<Q, AutoLock>;

        /// @brief @ref Modifier constraint.
        using Operands = util::meta::Traits<Q>;

        /// @brief @ref ecstasy::query::Queryable "Queryable" constaint.
        using QueryData = util::meta::add_optional_t<queryable_data_t<Q>>;

        ///
        /// @brief Construct a new Maybe Queryable modifier.
        ///
        /// @param[in] internal wrapped queryable.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        Maybe(Q &internal) : _internal(internal)
        {
            adjustMask(getQueryableMask(internal).size());
        }

        ///
        /// @brief Get the flipped mask of the internal queryable.
        ///
        /// @note @ref ecstasy::query::Queryable "Queryable" constraint.
        ///
        /// @return const util::BitSet& Flipped bitset.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        [[nodiscard]] constexpr const util::BitSet &getMask() const noexcept
        {
            return _mask;
        }

        ///
        /// @brief Get a std::optional filled with the data at index @p index if existing.
        ///
        /// @note @ref ecstasy::query::Queryable "Queryable" constraint.
        /// @warning May throw exceptions, look at the @b Internal type equivalent method documentation.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return @ref QueryData A std::optional filled with the data at index @p index if existing.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        [[nodiscard]] constexpr QueryData getQueryData(size_t index)
        {
            if (index < getQueryableMask(_internal).size() && getQueryableMask(_internal)[index])
                return QueryData{getQueryableData(_internal, index)};
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
        /// @brief Wrapped queryable.
        Internal _internal;
        /// @brief Mask of the internal queryable.
        util::BitSet _mask;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIER_MAYBE_HPP_ */
