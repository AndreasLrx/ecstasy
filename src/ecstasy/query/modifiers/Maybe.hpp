///
/// @file Maybe.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-24
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_MODIFIER_MAYBE_HPP_
#define ECSTASY_QUERY_MODIFIER_MAYBE_HPP_

#include <type_traits>

#include "Modifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "util/BitSet.hpp"

namespace ecstasy::query::modifier
{
    ///
    /// @brief Query modifier which returns a pointer to the data if existing or a nullptr.
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
        /// @brief @ref Queryable constaint.
        using QueryData = std::add_pointer_t<typename Internal::QueryData>;

        ///
        /// @brief Construct a new Not Queryable modifier.
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
        /// @note @ref Queryable constraint.
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
        /// @brief Get a pointer to the internal query data at the given index if existing, or @p nullptr otherwise.
        ///
        /// @note @ref Queryable constraint.
        /// @warning May throw exceptions, look at the @b Internal type equivalent function documentation.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return QueryData A pointer to the data at index @p index if existing, or @p nullptr otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        constexpr QueryData getQueryData(size_t index)
        {
            if (index < _internal.getMask().size() && _internal.getMask()[index])
                return &_internal.getQueryData(index);
            return nullptr;
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
