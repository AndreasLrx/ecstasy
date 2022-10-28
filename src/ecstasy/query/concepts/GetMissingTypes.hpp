///
/// @file GetMissingTypes.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-26
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_GETMISSINGTYPES_HPP_
#define ECSTASY_QUERY_CONCEPTS_GETMISSINGTYPES_HPP_

#include "util.hpp"

namespace ecstasy::query
{
    ///
    /// @brief Proxy structure to retrieve all types @b Ts which are in the required types @b Rs and not in the
    /// available types @b As. Another black magic template usage.
    /// Usage: available_types<As...>::get_missings_t<Rs...> -> std::tuple<Ts...>.
    ///
    /// @tparam Availables Available types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-26)
    ///
    template <typename... Availables>
    struct available_types {
      private:
        /// @internal
        /// @brief Primary template to filter the missing types, keeping the one in Required but not in Availables.
        ///
        /// @tparam IsPresent Whether or not the current type (called Required) is present in the Availables types.
        /// @tparam Required Current evaluated type.
        /// @tparam Missings All missing types (which belongs to the required types but not to the Availables types).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-26)
        ///
        template <bool IsPresent, typename Required, typename... Missings>
        struct missing {
            ///
            /// @brief Primary template for the final type. Specialization is made depending on the @p IsPresent value.
            /// If @p RequiredTypes is empty, it stops here and the resulting type is a std::tuple<Missings...> with @p
            /// Required when @p IsPresent is false.
            /// Otherwise a recursive @a call is made with the following parameters.
            /// @p IsPresent is set to true if the first type in @p RequiredTypes is not present in @p Availables.
            /// @p Required is the first type in @p RequiredTypes.
            /// @p Missings is concatenated with @p Required if @p IsPresent is false (otherwise, @p Missings doesn't
            /// change).
            /// @p RequiredTypes is forwarded minus the first type in the current @p RequriedTypes.
            ///
            /// @note The resulting type can be accessed by missing_tuple<T>::type or using @ref missings_tuple_t<T>.
            ///
            /// @tparam RequiredTypes Pack of required types.
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-26)
            ///
            template <typename... RequiredTypes>
            struct missings_tuple {
                // clang-format off
                using type = std::conditional<IsPresent,
                    /// Type already present, dont include it
                    std::tuple<Missings...>,
                    /// Type missing, include it
                    std::tuple<Missings..., Required>>::type;
                // clang-format on
            };

            /// @copydoc missings_tuple
            template <typename NextRequired, typename... RequiredTypes>
            struct missings_tuple<NextRequired, RequiredTypes...> {
                // clang-format off
                using type = std::conditional_t<IsPresent, 
                    /// Type already present, dont include it
                    typename missing<contains<NextRequired, Availables...>(),   // IsPresent
                                    NextRequired,                               // Required
                                    Missings...>::                              // Missings
                                        template missings_tuple<RequiredTypes...>::type,
                    /// Type missing, include it
                    typename missing<contains<NextRequired, Availables...>(),   // IsPresent
                                    NextRequired,                               // Required
                                    Missings..., Required>::                    // Missings
                                        template missings_tuple<RequiredTypes...>::type>;
                // clang-format on
            };

            template <typename... Qs>
            using missings_tuple_t = missings_tuple<Qs...>::type;
        };

      public:
        ///
        /// @brief Declare a tuple type containing all the types present in @p Required but not in @p Availables.
        ///
        /// @tparam Required Required types.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        template <typename... Required>
        struct get_missings {
            using type = std::tuple<void>;
        };

        template <typename FirstRequired, typename... Required>
        struct get_missings<FirstRequired, Required...> {
            using type = typename missing<contains<FirstRequired, Availables...>(),
                FirstRequired>::template missings_tuple_t<Required...>;
        };

        ///
        /// @brief Helper for get_missings<Qs...>::type.
        ///
        /// @tparam Qs Required types.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        template <typename... Qs>
        using get_missings_t = get_missings<Qs...>::type;
    };
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_GETMISSINGTYPES_HPP_ */
