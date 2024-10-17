///
/// @file DeletionStack.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Deletion stack implementation.
/// @version 1.0.0
/// @date 2022-10-21
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "DeletionStack.hpp"
#include "ecstasy/registry/Registry.hpp"

namespace ecstasy
{
    DeletionStack::DeletionStack(Registry &registry) : _registry(registry)
    {
    }

    DeletionStack::~DeletionStack()
    {
        if (_killed.size() > 0)
            _registry.eraseEntities(_killed);
    }

    void DeletionStack::push(Entity entity)
    {
        _killed.push_back(entity);
    }

    [[nodiscard]] size_t DeletionStack::size() const noexcept
    {
        return _killed.size();
    }
} // namespace ecstasy
