///
/// @file System.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_SYSTEM_ISYSTEM_HPP_
#define ECSTASY_SYSTEM_ISYSTEM_HPP_

namespace ecstasy
{
    /// @brief Forward declaration of Registry class.
    class Registry;

    ///
    /// @brief System interface, base class of all systems.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-17)
    ///
    class ISystem {
      public:
        /// @brief Default destructor.
        virtual ~ISystem() = default;

        ///
        /// @brief Run the system.
        ///
        /// @param[in] registry Reference to the registry running the system.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        virtual void run(Registry &registry) = 0;
    };
} // namespace ecstasy

#endif /* !ECSTASY_SYSTEM_ISYSTEM_HPP_ */
