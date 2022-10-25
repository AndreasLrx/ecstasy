///
/// @file Queryable.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022
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

namespace ecstasy
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
    concept Queryable = requires(Q &queryable, Q const &cqueryable, std::size_t index)
    {
        /// Data stored in the queryable.
        typename Q::QueryData;

        // clang-format off
        /// Const method returning the mask where all bits set to 1 means a query data at the bit index is available.
        { cqueryable.getMask() } -> std::same_as<util::BitSet const &>;

        /// Fetch the query data at the given index.
        { queryable.getQueryData(index) } -> std::same_as<typename Q::QueryData>;
        // clang-format on
    };
} // namespace ecstasy

#endif /* !ECSTASY_QUERY_CONCEPTS_QUERYABLE_HPP_ */
