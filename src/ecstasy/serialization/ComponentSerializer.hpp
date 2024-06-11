///
/// @file ComponentSerializers.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-19
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_SERIALIZATION_COMPONENT_SERIALIZERS_HPP_
#define ECSTASY_SERIALIZATION_COMPONENT_SERIALIZERS_HPP_

#include "ecstasy/config.hpp"
#include "ecstasy/serialization/RawSerializer.hpp"
#include "util/meta/Traits.hpp"
#include "ecstasy/serialization/is_serializable.hpp"

#ifndef ECSTASY_SERIALIZERS
    #define ECSTASY_SERIALIZERS util::meta::Traits<ecstasy::serialization::RawSerializer>
#endif

namespace ecstasy::serialization
{
    class ISerializer;

    using Serializers = ECSTASY_SERIALIZERS;

    ///
    /// @brief Serialize a component with the given serializer. Serializer is passed as an ISerializer but with its
    /// type_info.
    ///
    /// @note This struct is used to counter the limit of the C++ language that doesn't allow to have a virtual
    /// template.
    ///
    /// @tparam S Registered serializer types wrapped inside an @ref util::meta::Traits.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-11)
    ///
    template <typename S>
    struct ComponentSerializer {
        static_assert(!std::is_same_v<S, util::meta::Traits<>>, "No serializer registered");
    };

    /// @copydoc ComponentSerializer
    template <typename S1, typename... Ss>
    struct ComponentSerializer<util::meta::Traits<S1, Ss...>> {
        ///
        /// @brief Serialize a component with the given serializer.
        ///
        /// @note This function will try to serialize the component with each serializer in the list, stopping when the
        /// targeted serializer is found (using @p stype).
        ///
        /// @tparam Comp Component type.
        ///
        /// @param[in] serializer Serializer to use.
        /// @param[in] stype Type of the serializer.
        /// @param[in] component Component to serialize.
        ///
        /// @return ISerializer& The serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        template <typename Comp>
        static ISerializer &serialize(ISerializer &serializer, const std::type_info &stype, const Comp &component)
        {
            (trySerialize<S1>(serializer, stype, component) || ... || trySerialize<Ss>(serializer, stype, component));
            return serializer;
        }

        ///
        /// @brief Try to serialize a component with the given serializer. The serialization is done if S is the same as
        /// @p stype.
        ///
        /// @tparam S Serializer type.
        /// @tparam Comp Component type.
        ///
        /// @param[in] serializer Serializer to use.
        /// @param[in] stype Type of the serializer searched.
        /// @param[in] component Component to serialize.
        ///
        /// @return bool True if the component was serialized, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        template <typename S, typename Comp>
        static bool trySerialize(ISerializer &serializer, const std::type_info &stype, const Comp &component)
        {
            if constexpr (is_serializable_v<S, Comp>) {
                if (stype == typeid(S)) {
                    dynamic_cast<S &>(serializer).saveEntityComponent(component);
                    return true;
                }
            }
            return false;
        }
    };

    ///
    /// @brief Serialize a component with the given serializer if possible.
    ///
    /// @tparam C Component type.
    ///
    /// @param[in] serializer Serializer to use.
    /// @param[in] stype Type of the serializer.
    /// @param[in] component Component to serialize.
    ///
    /// @return ISerializer& The serializer.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-11)
    ///
    template <typename C>
    ISerializer &serialize(ISerializer &serializer, const std::type_info &stype, const C &component)
    {
        return ComponentSerializer<Serializers>::serialize(serializer, stype, component);
    }

} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_COMPONENT_SERIALIZERS_HPP_ */
