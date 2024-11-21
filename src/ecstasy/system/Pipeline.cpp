///
/// @file Pipeline.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "Pipeline.hpp"
#include "ecstasy/registry/Registry.hpp"

namespace ecstasy
{

    Pipeline::SystemIterator Pipeline::Phase::begin() const noexcept
    {
        return _pipeline._systemsIds.begin() + static_cast<long>(_begin);
    }

    Pipeline::SystemIterator Pipeline::Phase::end() const noexcept
    {
        return _pipeline._systemsIds.begin() + static_cast<long>(end_idx());
    }

    void Pipeline::Phase::run() const
    {
        for (auto it = begin(); it < end(); ++it) {
            _pipeline._registry.runSystem(*it);
        }
    }

    Pipeline::Pipeline(Registry &registry) : _registry(registry)
    {
    }

    void Pipeline::addSystem(std::type_index system, Pipeline::PhaseId phaseId)
    {
        Phase &phase = getPhase(phaseId);

        // Phase end is always after the last valid system (like end() in std::vector)
        _systemsIds.insert(_systemsIds.begin() + static_cast<long>(phase.end_idx()), system);
        ++phase._size;

        /// Need to update all phases iterators
        auto phaseIt = _phases.find(phaseId);
        size_t nextPhaseBegin = phase.end_idx();

        for (++phaseIt; phaseIt != _phases.end(); ++phaseIt) {
            phaseIt->second._begin = nextPhaseBegin;
            nextPhaseBegin = phaseIt->second.end_idx();
        }
    }

    void Pipeline::run() const
    {
        for (auto &phase : _phases) {
            phase.second.run();
        }
    }

    void Pipeline::run(Pipeline::PhaseId phase) const
    {
        auto phaseIt = _phases.find(phase);

        if (phaseIt == _phases.end())
            return;

        phaseIt->second.run();
    }

    Pipeline::Phase &Pipeline::getPhase(Pipeline::PhaseId id, bool autoRegister)
    {
        if (autoRegister)
            return registerPhase(id);
        auto it = _phases.find(id);

        if (it != _phases.end())
            return it->second;
        throw std::out_of_range("Phase not found");
    }

    const Pipeline::Phase &Pipeline::getSystemPhase(std::type_index system) const
    {
        auto it = std::find(_systemsIds.begin(), _systemsIds.end(), system);

        if (it == _systemsIds.end())
            throw std::out_of_range("System not found in any phase");
        for (auto &phase : _phases) {
            if (it >= phase.second.begin() && it < phase.second.end())
                return phase.second;
        }
        throw std::out_of_range("System not found in any phase");
    }

    Pipeline::Phase &Pipeline::registerPhase(Pipeline::PhaseId id)
    {
        auto it = _phases.find(id);

        if (it != _phases.end())
            return it->second;

        it = _phases.emplace(id, Phase(*this, id)).first;
        Phase &phase = it->second;

        // No phase preceeding this one, set begin to the start of the systems list
        if (it == _phases.begin()) {
            phase._begin = 0;
        } else {
            phase._begin = std::prev(it)->second.end_idx();
        }
        // No need to update following phases since our phase is empty
        return phase;
    }

} // namespace ecstasy
