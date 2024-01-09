///
/// @file Queryable.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_QUERYABLE_HPP_
#define ECSTASY_QUERY_CONCEPTS_QUERYABLE_HPP_

#include <concepts>
#include <cstddef>

namespace util
{
    class BitSet;
}

namespace ecstasy::query
{
    ///
    /// @brief Defines a type that can be queried.
    ///
    /// @tparam Q Evaluated Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename Q>
    concept Queryable = requires(Q &queryable, Q const &cqueryable, std::size_t index) {
        /// Data stored in the queryable.
        typename Q::QueryData;

        // clang-format off
        /// Const method returning the mask where all bits set to 1 means a query data at the bit index is available.
        { cqueryable.getMask() } -> std::same_as<util::BitSet const &>;

        /// Fetch the query data at the given index.
        { queryable.getQueryData(index) } -> std::same_as<typename Q::QueryData>;
        // clang-format on
    };

    ///
    /// @brief Checks if the given type match the @ref ecstasy::query::Queryable concept.
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-30)
    ///
    template <typename T>
    struct is_queryable : public std::false_type {};

    /// @copydoc is_queryable
    template <Queryable T>
    struct is_queryable<T> : public std::true_type {};

    ///
    /// @brief Helper for @ref is_queryable "is_queryable<T>::value".
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-30)
    ///
    template <typename T>
    static constexpr inline bool is_queryable_v = is_queryable<T>::value;

} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_QUERYABLE_HPP_ */
