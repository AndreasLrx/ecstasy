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
#include "ecstasy/serialization/traits/can_load_type.hpp"
#include "ecstasy/serialization/traits/can_save_type.hpp"
#include "ecstasy/serialization/traits/can_update_type.hpp"

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
        /// @brief Save a component with the given serializer.
        ///
        /// @note This function will try to save the component with each serializer in the list, stopping when the
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
        static ISerializer &save(ISerializer &serializer, const std::type_info &stype, const Comp &component)
        {
            (trySave<S1>(serializer, stype, component) || ... || trySave<Ss>(serializer, stype, component));
            return serializer;
        }

        ///
        /// @brief Try to save a component with the given serializer. The serialization is done if S is the same as
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
        static bool trySave(ISerializer &serializer, const std::type_info &stype, const Comp &component)
        {
            if constexpr (traits::can_save_type_v<S, Comp>) {
                if (stype == typeid(S)) {
                    dynamic_cast<S &>(serializer).saveEntityComponent(component);
                    return true;
                }
            }
            return false;
        }

        ///
        /// @brief Update a component with the given serializer.
        ///
        /// @tparam Comp Component type.
        ///
        /// @param[in] serializer Serializer to use.
        /// @param[in] stype Type of the serializer.
        /// @param[in] component Component to update.
        ///
        /// @return ISerializer& The serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-25)
        ///
        template <typename Comp>
        static ISerializer &update(ISerializer &serializer, const std::type_info &stype, Comp &component)
        {
            (tryUpdate<S1>(serializer, stype, component) || ... || tryUpdate<Ss>(serializer, stype, component));
            return serializer;
        }

        ///
        /// @brief Try to update a component with the given serializer.
        ///
        /// @tparam S Serializer type.
        /// @tparam Comp Component type.
        ///
        /// @param[in] serializer Serializer to use.
        /// @param[in] stype Type of the serializer.
        /// @param[in] component Component to update.
        ///
        /// @return bool True if the component was updated, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-25)
        ///
        template <typename S, typename Comp>
        static bool tryUpdate(ISerializer &serializer, const std::type_info &stype, Comp &component)
        {
            if constexpr (traits::can_update_type_v<S, Comp>) {
                if (stype == typeid(S)) {
                    dynamic_cast<S &>(serializer).update(component);
                    return true;
                }
            }
            return false;
        }

        ///
        /// @brief Load a component with the given serializer for a specifc entity.
        ///
        /// @tparam Storage Storage type.
        ///
        /// @param[in] serializer Serializer to use.
        /// @param[in] stype Type of the serializer.
        /// @param[in] storage Storage to use.
        /// @param[in] entityId Entity id to which the storage must be loaded.
        ///
        /// @return ISerializer& The serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-25)
        ///
        template <typename Storage>
        static ISerializer &load(
            ISerializer &serializer, const std::type_info &stype, Storage &storage, size_t entityId)
        {
            (tryLoad<S1>(serializer, stype, storage, entityId) || ...
                || tryLoad<Ss>(serializer, stype, storage, entityId));
            return serializer;
        }

        ///
        /// @brief Try to load a component with the given serializer for a specific entity.
        ///
        /// @tparam S Serializer type.
        /// @tparam Storage Storage type.
        ///
        /// @param[in] serializer Serializer to use.
        /// @param[in] stype Type of the serializer.
        /// @param[in] storage Storage to use.
        /// @param[in] entityId Entity id to which the storage must be loaded.
        ///
        /// @return bool True if the component was loaded, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-25)
        ///
        template <typename S, typename Storage>
        static bool tryLoad(ISerializer &serializer, const std::type_info &stype, Storage &storage, size_t entityId)
        {
            if constexpr (traits::can_load_type_v<S, typename Storage::Component>
                && std::movable<typename Storage::Component>) {
                if (stype == typeid(S)) {
                    storage.insert(
                        entityId, dynamic_cast<S &>(serializer).template load<typename Storage::Component>());
                    return true;
                }
            }
            return false;
        }
    };

    ///
    /// @brief Save a component with the given serializer if possible.
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
    ISerializer &save(ISerializer &serializer, const std::type_info &stype, const C &component)
    {
        return ComponentSerializer<Serializers>::save(serializer, stype, component);
    }

    ///
    /// @brief Update a component with the given serializer if possible.
    ///
    /// @tparam C Component type.
    ///
    /// @param[in] serializer Serializer to use.
    /// @param[in] stype Type of the serializer.
    /// @param[in] component Component to update.
    ///
    /// @return ISerializer& The serializer.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-25)
    ///
    template <typename C>
    ISerializer &update(ISerializer &serializer, const std::type_info &stype, C &component)
    {
        return ComponentSerializer<Serializers>::update(serializer, stype, component);
    }

    ///
    /// @brief Load a component with the given serializer if possible.
    ///
    /// @tparam Storage Storage type.
    ///
    /// @param[in] serializer Serializer to use.
    /// @param[in] stype Type of the serializer.
    /// @param[in] storage Storage to use.
    /// @param[in] entityId Entity id to which the storage must be loaded.
    ///
    /// @return ISerializer& The serializer.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-25)
    ///
    template <typename Storage>
    ISerializer &load(ISerializer &serializer, const std::type_info &stype, Storage &storage, size_t entityId)
    {
        return ComponentSerializer<Serializers>::load(serializer, stype, storage, entityId);
    }

} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_COMPONENT_SERIALIZERS_HPP_ */
