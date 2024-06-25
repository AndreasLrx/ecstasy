///
/// @file IStorage.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-19
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_STORAGE_ISTORAGE_HPP_
#define ECSTASY_STORAGE_ISTORAGE_HPP_

#include <span>

#include "ecstasy/config.hpp"
#include "util/meta/Traits.hpp"

#ifdef ECSTASY_LOCKABLE_STORAGES
    #include "ecstasy/thread/SharedRecursiveMutex.hpp"
#endif

namespace util
{
    class BitSet;
}

namespace ecstasy
{

    namespace serialization
    {
        class ISerializer;
    }
    class Entity;

    ///
    /// @brief Base class of all components storage.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-19)
    ///
    class IStorage
#ifdef ECSTASY_LOCKABLE_STORAGES
        : public thread::SharedRecursiveMutex
#endif
    {
      public:
        virtual ~IStorage() = default;

        ///
        /// @brief Get the Component Mask.
        ///
        /// @note Each bit set to true mean the entity at the bit index has a component in the storage.
        /// @warning The mask might be smaller than the entity count.
        ///
        /// @return const util::BitSet& Component mask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        virtual constexpr const util::BitSet &getMask() const = 0;

        ///
        /// @brief Erase the components attached to the given entities.
        ///
        /// @param[in] entities Target entities.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        virtual void erase(std::span<Entity> entities) = 0;

        ///
        /// @brief Test if the entity index match a @b Component instance.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return bool True if the entity has a component, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        virtual bool contains(size_t index) const noexcept = 0;

        ///
        /// @brief Get the Component stored type infos.
        ///
        /// @return const std::type_info& Type informations of the component stored in the storage.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        virtual const std::type_info &getComponentTypeInfos() const noexcept = 0;

        ///
        /// @brief Save an entity component.
        ///
        /// @note The type_info is used to counter the impossibility to use template virtual methods.
        ///
        /// @param[in] serializer Serializer object.
        /// @param[in] stype Type informations of the serializer.
        /// @param[in] entityId Entity index.
        ///
        /// @return serialization::ISerializer& Serializer object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        virtual serialization::ISerializer &save(
            serialization::ISerializer &serializer, const std::type_info &stype, size_t entityId) const = 0;

        ///
        /// @brief Load/update an entity component.
        ///
        /// @param[in] serializer Serializer object.
        /// @param[in] stype Type informations of the serializer.
        /// @param[in] entityId Entity index.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-25)
        ///
        virtual void load(serialization::ISerializer &serializer, const std::type_info &stype, size_t entityId) = 0;
    };

} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_ISTORAGE_HPP_ */
