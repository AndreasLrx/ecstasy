///
/// @file IStorage.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Storage interface for components.
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
        [[nodiscard]] virtual constexpr const util::BitSet &getMask() const noexcept = 0;

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
        [[nodiscard]] virtual bool contains(size_t index) const noexcept = 0;

        ///
        /// @brief Get the Component stored type infos.
        ///
        /// @return const std::type_info& Type informations of the component stored in the storage.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        [[nodiscard]] virtual const std::type_info &getComponentTypeInfos() const noexcept = 0;
    };

} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_ISTORAGE_HPP_ */
