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

#include <optional>
#include <type_traits>

#include "Modifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "util/BitSet.hpp"

namespace ecstasy::query::modifier
{
    ///
    /// @brief Query modifier which returns a std::optional filled when the data is existing.
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
      private:
        template <typename T>
        struct optional_content {
            using type = T;
        };

        template <typename T>
        struct optional_content<T &> {
            using type = std::reference_wrapper<T>;
        };

        template <typename T>
        using optional_content_t = optional_content<T>::type;

      public:
        /// @brief Wrapped queryable.
        using Internal = Q;
        /// @brief @ref Queryable constaint.
        using QueryData = std::optional<optional_content_t<typename Internal::QueryData>>;

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
        /// @brief Get a std::optional filled with the data at index @p index if existing.
        ///
        /// @note @ref Queryable constraint.
        /// @warning May throw exceptions, look at the @b Internal type equivalent function documentation.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return QueryData A std::optional filled with the data at index @p index if existing.
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
