/*
** EPITECH PROJECT, 2022
** ecstasy
** File description:
** StorageConcepts
*/

#ifndef ECSTASY_QUERY_QUERYCONCEPTS_HPP_
#define ECSTASY_QUERY_QUERYCONCEPTS_HPP_

#include <concepts>
#include <type_traits>

namespace util
{
    class BitSet;
}

namespace ecstasy
{
    /// Implemented by types that can be queried.
    template <typename Q>
    concept Queryable = requires(Q &queryable, Q const &cqueryable, std::size_t index)
    {
        typename Q::QueryData;

        // clang-format off
        { cqueryable.getMask() } -> std::same_as<util::BitSet const &>;
        { queryable.getQueryData(index) } -> std::same_as<typename Q::QueryData>;
        // clang-format on
    };

    template <typename T, typename... Ts>
    constexpr bool contains()
    {
        return std::disjunction_v<std::is_same<T, Ts>...>;
    }
} // namespace ecstasy
#endif /* !ECSTASY_QUERY_QUERYCONCEPTS_HPP_ */
