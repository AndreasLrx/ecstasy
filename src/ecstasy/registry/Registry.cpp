///
/// @file Registry.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "Registry.hpp"

namespace ecstasy
{
    void Registry::runSystems()
    {
        for (auto &[type, system] : _systems.getInner())
            system->run(*this);
    }
} // namespace ecstasy