///
/// @file Query.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-20
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "QueryConcepts.hpp"
#include "ecstasy/storage/IStorage.hpp"
#include "ecstasy/storage/StorageConcepts.hpp"

#include "Query.hpp"
#include "ecstasy/resource/entity/Entities.hpp"

namespace ecstasy
{
    Query::Query(Entities &entities) : _mask(entities.getMask())
    {
    }

    Query &Query::where(IStorage &storage)
    {
        _mask &= storage.getMask();
        return *this;
    }
} // namespace ecstasy