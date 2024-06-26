///
/// @file Serializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-04-29
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_SERIALIZATION_SERIALIZER_HPP_
#define ECSTASY_SERIALIZATION_SERIALIZER_HPP_

#include "ecstasy/serialization/ISerializer.hpp"

namespace ecstasy::serialization
{

    ///
    /// @brief Serializer class.
    ///
    /// @tparam S Child serializer type. This is required to call the sub serializer class methods because template and
    /// virtual methods cannot be mixed.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-30)
    ///
    template <typename S>
    class Serializer : public ISerializer {
      protected:
        /// @brief Parent serializer type, used for inheriting classes.
        using Parent = Serializer<S>;

      public:
        ///
        /// @brief Check if a type is constructible from the serializer.
        ///
        /// @note The type must be constructible from a non const reference to the inner serializer.
        ///
        /// @tparam U Type to check.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        static constexpr bool is_constructible = std::is_constructible_v<U, std::add_lvalue_reference_t<S>>;

        ///
        /// @brief Construct a new Serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        Serializer() = default;

        ///
        /// @brief Destroy the Serializer
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        ~Serializer() override = default;

        ///
        /// @brief Get a reference to the inner serializer.
        ///
        /// @return constexpr S& Reference to the inner serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        constexpr S &inner()
        {
            return reinterpret_cast<S &>(*this);
        }

        ///
        /// @brief Save an object to the serializer.
        ///
        /// @tparam U Type of the object to save.
        ///
        /// @param[in] object Object to save.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        S &save(const U &object)
        {
            return object >> inner();
        }

        ///
        /// @brief Load an object from the serializer.
        ///
        /// @note This construct a new object and consume the associated data from the serializer.
        ///
        /// @tparam U Type of the object to load.
        ///
        /// @return U Loaded object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        U load()
        {
            if constexpr (is_constructible<U>) {
                return U(inner());
            } else if constexpr (std::is_default_constructible_v<U>) {
                U object;
                inner().update(object);
                return object;
            } else {
                static_assert(!(is_constructible<U> || std::is_default_constructible_v<U>),
                    "Type cannot be loaded. It must be default constructible or constructible from a serializer");
            }
        }

        ///
        /// @brief Update an existing object from the serializer.
        ///
        /// @note If the object is fundamental, it will use the assignment operator, otherwise the << operator is
        /// expected to be implemented in U.
        ///
        /// @tparam U Type of the object to update.
        ///
        /// @param[in] object Existing object to update.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        S &update(U &object)
        {
            if constexpr (std::is_fundamental_v<U>) {
                object = inner().template load<U>();
            } else {
                object << inner();
            }
            return inner();
        }

        ///
        /// @brief Operator overload to simplify the save method.
        ///
        /// @tparam U Type of the object to save.
        ///
        /// @param[in] object Object to save.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        S &operator<<(const U &object)
        {
            return inner().save(object);
        }

        ///
        /// @brief Operator overload to simplify the update method.
        ///
        /// @tparam U Type of the object to update.
        ///
        /// @param[in] object  Object to update.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        S &operator>>(U &object)
        {
            return inner().update(object);
        }
    };
} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_SERIALIZER_HPP_ */
