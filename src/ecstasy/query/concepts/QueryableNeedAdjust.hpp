///
/// @file QueryableNeedAdjust.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Defines a Queryable object type which has the adjustMask() implemented.
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_QUERYABLENEEDADJUST_HPP_
#define ECSTASY_QUERY_CONCEPTS_QUERYABLENEEDADJUST_HPP_

#include "Queryable.hpp"

namespace ecstasy::query
{
    ///
    /// @brief Defines a Queryable object type which has the adjustMask() implemented.
    ///
    /// @tparam Q Evaluated Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename Q>
    concept QueryableObjectNeedAdjust = requires(Q &queryable, std::size_t maxSize) {
        /// Ensure the type is a queryable
        requires QueryableObject<Q>;

        // clang-format off
        { queryable.adjustMask(maxSize) } -> std::same_as<void>;
        // clang-format on
    };

    ///
    /// @brief Defines a Queryable wrapper type which has the adjustMask() implemented.
    ///
    /// @tparam Q Evaluated Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    template <typename Q>
    concept QueryableWrapperNeedAdjust = requires(Q &queryable, std::size_t maxSize) {
        /// Ensure the type is a queryable
        requires QueryableWrapper<Q>;

        // clang-format off
        { queryable->adjustMask(maxSize) } -> std::same_as<void>;
        // clang-format on
    };

    ///
    /// @brief Defines a Queryable type which has the adjustMask() implemented.
    ///
    /// @tparam Q Evaluated Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    template <typename Q>
    concept QueryableNeedAdjust = QueryableObjectNeedAdjust<Q> || QueryableWrapperNeedAdjust<Q>;

    ///
    /// @brief Adjust the queryable mask if needed.
    ///
    /// @note This is mainly for modifiers purposes.
    /// @note This will call @ref adjustMask() method if implemented (see @ref ecstasy::query::QueryableNeedAdjust
    /// "QueryableNeedAdjust" concept).
    ///
    /// @tparam T Type of the queryable.
    ///
    /// @param[in] queryable queryable value.
    /// @param[in] maxSize maxSize of the mask that will be compared to the queryable mask.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename T>
    constexpr void adjustMask([[maybe_unused]] T &queryable, [[maybe_unused]] size_t maxSize) noexcept
    {
    }

    /// @copydoc adjustMask()
    template <QueryableNeedAdjust Q>
    constexpr void adjustMask(Q &queryable, size_t maxSize)
    {
        if constexpr (QueryableWrapper<Q>)
            queryable->adjustMask(maxSize);
        else
            queryable.adjustMask(maxSize);
    }

    ///
    /// @brief Checks if the given type match the @ref ecstasy::query::QueryableNeedAdjust "QueryableNeedAdjust"
    /// concept.
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-30)
    ///
    template <typename T>
    struct is_queryable_with_adjust : public std::false_type {};

    /// @copydoc is_queryable_with_adjust
    template <QueryableNeedAdjust T>
    struct is_queryable_with_adjust<T> : public std::true_type {};

    ///
    /// @brief Helper for @ref is_queryable_with_adjust
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-30)
    ///
    template <typename T>
    static constexpr inline bool is_queryable_with_adjust_v = is_queryable_with_adjust<T>::value;
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_QUERYABLENEEDADJUST_HPP_ */
