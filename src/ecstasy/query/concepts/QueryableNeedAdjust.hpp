///
/// @file QueryableNeedAdjust.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_QUERYABLENEEDADJUST_HPP_
#define ECSTASY_QUERY_CONCEPTS_QUERYABLENEEDADJUST_HPP_

#include "Queryable.hpp"

namespace ecstasy::query
{
    ///
    /// @brief Defines a Queryable type which has the @ref adjustMask() implemented.
    ///
    /// @tparam Q Evaluated Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename Q>
    concept QueryableNeedAdjust = requires(Q &queryable, std::size_t maxSize)
    {
        /// Ensure the type is a queryable
        requires Queryable<Q>;

        // clang-format off
        { queryable.adjustMask(maxSize) } -> std::same_as<void>;
        // clang-format on
    };

    ///
    /// @brief Adjust the queryable mask if needed.
    ///
    /// @note This is mainly for modifiers purposes.
    /// @note This will call @ref adjustMask() method if implemented (see @ref QueryableNeedAdjust concept).
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
    constexpr void adjustMask(T &queryable, size_t maxSize)
    {
        (void)queryable;
        (void)maxSize;
    }

    /// @copydoc adjustMask()
    template <QueryableNeedAdjust Q>
    constexpr void adjustMask(Q &queryable, size_t maxSize)
    {
        queryable.adjustMask(maxSize);
    }

    ///
    /// @brief Checks if the given type match the @ref QueryableNeedAdjust concept.
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-30)
    ///
    template <typename T>
    struct is_queryable_with_adjust : public std::false_type {
    };

    /// @copydoc is_queryable_with_adjust
    template <QueryableNeedAdjust T>
    struct is_queryable_with_adjust<T> : public std::true_type {
    };

    ///
    /// @brief Helper for @ref is_queryable_with_adjust<T>::value.
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
