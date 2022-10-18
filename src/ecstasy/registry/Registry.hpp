///
/// @file Registry.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_REGISTRY_REGISTRY_HPP_
#define ECSTASY_REGISTRY_REGISTRY_HPP_

#include "ecstasy/storage/Instances.hpp"
#include "ecstasy/system/ISystem.hpp"

namespace ecstasy
{
    class Resource;

    class Registry {
      public:
        ///
        /// @brief Construct a new Registry.
        ///
        /// @param[in] addEntities Whether or not the @ref Entities @ref Resource should be declared or not in the
        /// registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        Registry(bool addEntities = true);

        /// @brief Default destructor.
        ~Registry() = default;

        ///
        /// @brief Add a new system in the registry.
        ///
        /// @tparam S System to add.
        /// @tparam Args The type of arguments to pass to the constructor of @b S.
        ///
        /// @param[in] args The arguments to pass to the constructor of @b S.
        ///
        /// @return S& newly created System.
        ///
        /// @throw std::logic_error If the system @b S was already present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<ISystem> S, typename... Args>
        S &addSystem(Args &&...args)
        {
            return _systems.emplace<S>(std::forward<Args>(args)...);
        }

        ///
        /// @brief Add a new resource in the registry.
        ///
        /// @tparam R Resource to add.
        /// @tparam Args The type of arguments to pass to the constructor of @b R.
        ///
        /// @param[in] args The arguments to pass to the constructor of @b R.
        ///
        /// @return R& newly created Resource.
        ///
        /// @throw std::logic_error If the resource @b R was already present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        template <std::derived_from<Resource> R, typename... Args>
        R &addResource(Args &&...args)
        {
            return _resources.emplace<R>(std::forward<Args>(args)...);
        }

        ///
        /// @brief Get the Resource of type @b R.
        ///
        /// @tparam R Type of the resource to fetch.
        ///
        /// @return R& Reference to an instance of type @b R.
        ///
        /// @throw std::logic_error If the resource @b R was not present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        template <std::derived_from<Resource> R>
        R &getResource()
        {
            return _resources.get<R>();
        }

        ///
        /// @brief Run a specific system from the registry.
        ///
        /// @tparam S System class to run.
        ///
        /// @throw std::logic_error If the system @b S is not present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<ISystem> S>
        void runSystem()
        {
            _systems.get<S>().run(*this);
        }

        ///
        /// @brief Run all systems present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        void runSystems();

      private:
        Instances<ISystem> _systems;
        Instances<Resource> _resources;
    };
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_REGISTRY_HPP_ */
