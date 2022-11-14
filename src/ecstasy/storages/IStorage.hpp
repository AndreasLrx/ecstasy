///
/// @file IStorage.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-19
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_STORAGE_ISTORAGE_HPP_
#define ECSTASY_STORAGE_ISTORAGE_HPP_

#include <span>

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
    class IStorage {
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
    };

} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_ISTORAGE_HPP_ */
