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

#include "ecstasy/thread/LockableView.hpp"

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
    /// @brief Get the query data type of a queryable object.
    /// By default, this is the QueryData type of the @ref QueryableObject.
    /// If Q is a const @ref ConstQueryableObject, then the ConstQueryData type is returned.
    /// If Q is a @ref QueryableWrapper, then the queryable_data is called on the wrapped type.
    ///
    /// @tparam Q Queryable object.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    template <Queryable Q>
    struct queryable_data {
        using type = typename Q::QueryData;
    };

    /// @copydoc queryable_data
    template <ConstQueryableObject Q>
    struct queryable_data<const Q> {
        using type = typename Q::ConstQueryData;
    };

    /// @copydoc queryable_data
    template <QueryableWrapper W>
    struct queryable_data<W> {
        using type = typename queryable_data<typename W::WrappedType>::type;
    };

    ///
    /// @brief Alias for the query data type of a queryable object.
    ///
    /// @tparam Q Queryable object.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    template <typename Q>
    using queryable_data_t = typename queryable_data<Q>::type;

    ///
    /// @brief Get the thread safe type of a queryable type.
    /// If @b ThreadSafe is false, @b Q is not @ref thread::Lockable the type is @b Q. If the type is lockable, then the
    /// type is a @ref thread::LockableView<Q>.
    ///
    /// @tparam Q Queryable type.
    /// @tparam ThreadSafe Whether the queryable should be thread safe or not.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-06)
    ///
    template <Queryable Q, bool ThreadSafe>
    struct thread_safe_queryable {
        using type = Q;
    };

    /// @copydoc thread_safe_queryable
    template <thread::Lockable Q>
    struct thread_safe_queryable<Q, true> {
        using type = thread::LockableView<Q>;
    };

    ///
    /// @brief Alias for the thread safe type of a queryable type.
    ///
    /// @tparam Q Queryable type.
    /// @tparam ThreadSafe Whether the queryable should be thread safe or not. Default to true.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-06)
    ///
    template <typename Q, bool ThreadSafe = true>
    using thread_safe_queryable_t = typename thread_safe_queryable<Q, ThreadSafe>::type;

    ///
    /// @brief Get the queryable type with the correct qualifiers.
    /// By default this is a reference to the type @b Q.
    /// If Q is @ref thread::Lockable and @b AutoLock is true, then the type is a @ref thread_safe_queryable_t<Q>. It is
    /// not a reference to the type because it will be implicitly constructed using the @ref thread::LockableView<Q>
    /// constructor (taking Q&). Meaning a @ref ThreadSafeQuery will have LockableView<Q> in its storages but Q& in its
    /// constructor.
    ///
    /// @tparam Q Queryable object.
    /// @tparam AutoLock Whether the queryable should be locked while used or not.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-04)
    ///
    template <Queryable Q, bool AutoLock = false>
    struct queryable_qualifiers {
        using type = Q &;
    };

    /// @copydoc queryable_qualifiers
    template <thread::Lockable Q>
    struct queryable_qualifiers<Q, true> {
        using type = thread_safe_queryable_t<Q, true>;
    };

    ///
    /// @brief Alias for the queryable type with the correct qualifiers.
    ///
    /// @tparam Q Queryable object.
    /// @tparam AutoLock Whether the queryable should be locked while used or not.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-04)
    ///
    template <typename Q, bool AutoLock = false>
    using queryable_qualifiers_t = typename queryable_qualifiers<Q, AutoLock>::type;

    ///
    /// @brief Get the mask of the queryable object.
    ///
    /// @note Required because the dot operator is not overloadable for @ref QueryableWrapper.
    ///
    /// @tparam Q Queryable object.
    ///
    /// @param[in] queryable Queryable object.
    ///
    /// @return constexpr const util::BitSet& Mask of the queryable object.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    template <Queryable Q>
    constexpr const util::BitSet &getQueryableMask(const Q &queryable)
    {
        if constexpr (QueryableWrapper<Q>)
            return queryable->getMask();
        else
            return queryable.getMask();
    }

    ///
    /// @brief Get the query data at the given index.
    ///
    /// @note Required because the dot operator is not overloadable for @ref QueryableWrapper.
    ///
    /// @tparam Q Queryable object.
    /// @param[in] queryable Queryable object.
    /// @param[in] index Index of the query data to fetch.
    ///
    /// @return constexpr @ref ecstasy::query::queryable_data_t<Q> Query data at the given index.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    template <Queryable Q>
    constexpr queryable_data_t<Q> getQueryableData(Q &queryable, size_t index)
    {
        if constexpr (QueryableWrapper<Q>)
            return queryable->getQueryData(index);
        else
            return queryable.getQueryData(index);
    }

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

    ///
    /// @brief Get the size of the allocator required for the given queryable types.
    ///
    /// @note Size is always 0 if @b AutoLock is false. Otherwise, it is the sum of the sizeof(Q) for each Lockable
    /// queryable Q.
    ///
    /// @tparam AutoLock Whether the queryable should be locked while used or not.
    /// @tparam Qs Queryable types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-07)
    ///
    template <bool AutoLock, Queryable... Qs>
    struct views_allocator_size : public std::integral_constant<size_t, 0> {};

    /// @copydoc views_allocator_size
    template <bool AutoLock, Queryable Q, Queryable... Qs>
    struct views_allocator_size<AutoLock, Q, Qs...>
        : public
          // clang-format off
        std::integral_constant<size_t, views_allocator_size<AutoLock, Qs...>::value> {};

    /// @copydoc thread_safe_queryable
    template <thread::Lockable Q, Queryable... Qs>
    struct views_allocator_size<true, Q, Qs...>
        : public
          // clang-format off
        std::integral_constant<size_t, 
                sizeof(thread_safe_queryable_t<Q, true>) + 
                views_allocator_size<true, Qs...>::value> {};
    // clang-format on

    ///
    /// @brief Helper for @ref views_allocator_size "views_allocator_size<AutoLock, Qs...>::value".
    ///
    /// @tparam AutoLock Whether the queryable should be locked while used or not.
    /// @tparam Qs Queryable types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-07)
    ///
    template <bool AutoLock, Queryable... Qs>
    size_t constexpr views_allocator_size_v = views_allocator_size<AutoLock, Qs...>::value;

} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_QUERYABLE_HPP_ */
