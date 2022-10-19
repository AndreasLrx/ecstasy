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

/// Sets the storage type to use for @b ComponentType, if not called @b MapStorage is used by default.
#define SET_COMPONENT_STORAGE(ComponentType, StorageType) \
    template <>                                           \
    struct GetComponentStorageType<ComponentType> {       \
        using Value = StorageType<ComponentType>;         \
    };

/// Forward declaration of map storage to use as default storage.
namespace ecstasy
{
    template <typename C>
    class MapStorage;
}

/// @internal
/// @note Use ecstasy::getStorageType instead.
template <typename C>
struct GetComponentStorageType {
    using Value = ecstasy::MapStorage<C>;
};

namespace ecstasy
{
    ///
    /// @brief Base class of all components storage.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-19)
    ///
    class IStorage {
      public:
        virtual ~IStorage() = default;
    };

    /// Get the storage type of a component, returns MapStorage by default.
    template <typename C>
    using getStorageType = typename GetComponentStorageType<C>::Value;
} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_ISTORAGE_HPP_ */
