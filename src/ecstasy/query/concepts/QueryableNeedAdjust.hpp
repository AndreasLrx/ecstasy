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
        Queryable<Q>;

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
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_QUERYABLENEEDADJUST_HPP_ */
