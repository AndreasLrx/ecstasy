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
    // clang-format off

    ///
    /// @brief Defines a type that can be queried.
    ///
    /// @tparam Q Evaluated Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename Q>
    concept QueryableObject = requires(Q &queryable, Q const &cqueryable, std::size_t index) {
        /// Data stored in the queryable.
        typename Q::QueryData;

        /// Const method returning the mask where all bits set to 1 means a query data at the bit index is available.
        { cqueryable.getMask() } -> std::same_as<util::BitSet const &>;

        /// Fetch the query data at the given index.
        { queryable.getQueryData(index) } -> std::same_as<typename Q::QueryData>;
    };

    ///
    /// @brief Defines a type that can be queried in a const context.
    ///
    /// @note This concept is a subset of @ref QueryableObject.
    /// @warning The type Q is not expected to be const qualified.
    ///
    /// @tparam Q Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    template <typename Q>
    concept ConstQueryableObject = QueryableObject<Q> && requires(const Q &cqueryable, std::size_t index) {
        /// Data stored in the queryable.
        typename Q::ConstQueryData;

        /// Fetch the const query data at the given index.
        { cqueryable.getQueryData(index) } -> std::same_as<typename Q::ConstQueryData>;
    };

    ///
    /// @brief Defines a type that can be queried through a wrapper.
    ///
    /// @note @ref LockableView is the reason of this concept.
    /// @note The wrapped type must be a @ref QueryableObject or a const @ref ConstQueryableObject.
    /// @warning The getQueryable method must be const qualified from the wrapper since the wrapper itself is not expected to be modified.
    /// 
    /// @tparam W Wrapper type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    template <typename W>
    concept QueryableWrapper = requires(W const &cwrapper) {
        /// Wrapped type.
        typename W::WrappedType;
        
        /// Const method returning the mask where all bits set to 1 means a query data at the bit index is available.
        { cwrapper->getMask() } -> std::same_as<util::BitSet const &>;
    } 
    && 
    (
        /// The wrapped type is a @ref QueryableObject.
        (
            QueryableObject<typename W::WrappedType> && 
            requires(W const &cwrapper, std::size_t index) {
                /// Fetch the query data at the given index.
                { cwrapper->getQueryData(index) } -> std::same_as<typename W::WrappedType::QueryData>;
            }
        )
        ||
        /// The wrapped type is a const @ref ConstQueryableObject.
        (
            std::is_const_v<typename W::WrappedType> &&
            ConstQueryableObject<std::remove_const_t<typename W::WrappedType>> && 
            requires(W const &cwrapper, std::size_t index) {
                /// Fetch the query data at the given index.
                { cwrapper->getQueryData(index) } -> std::same_as<typename W::WrappedType::ConstQueryData>;
            }
        )
    );
    // clang-format on

    ///
    /// @brief Defines a type that can be queried.
    ///
    /// @note This concept is a combination of @ref QueryableObject, @ref ConstQueryableObject and @ref
    /// QueryableWrapper.
    ///
    /// @tparam Q Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    template <typename Q>
    concept Queryable = QueryableObject<Q>
        || (std::is_const_v<Q> && ConstQueryableObject<std::remove_const_t<Q>>) || QueryableWrapper<Q>;


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
