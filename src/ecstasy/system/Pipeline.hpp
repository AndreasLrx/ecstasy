///
/// @file Pipeline.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_SYSTEM_PIPELINE_HPP_
#define ECSTASY_SYSTEM_PIPELINE_HPP_

#include <map>
#include <typeindex>
#include <vector>
#include "ecstasy/system/ISystem.hpp"
#include "ecstasy/system/Timer.hpp"

namespace ecstasy
{
    /// @brief Forward declaration of Registry class.
    class Registry;

    ///
    /// @brief Pipeline of systems to orchestrate the execution of systems.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-11-21)
    ///
    class Pipeline {
      public:
        /// @brief Type of system iterator.
        using SystemIterator = std::vector<std::type_index>::const_iterator;
        /// @brief Type of phase identifiers.
        using PhaseId = std::size_t;

        ///
        /// @brief Some predefined phases
        ///
        /// @note Theses phases are inspired from the default ones in
        /// [flecs](https://www.flecs.dev/flecs/md_docs_2DesignWithFlecs.html#selecting-a-phase)
        ///
        /// @note The values are arbitrary and can be changed, they allow to insert custom phases between them.
        ///
        /// @since 1.0.0 (2024-11-21)
        ///
        enum class PredefinedPhases : std::size_t {
            OnLoad = 100,     ///< Load entities into the registry. (Ex: Keyboard inputs)
            PostLoad = 200,   ///< After loading entities into the registry. (Ex: Convert inputs to game actions)
            PreUpdate = 300,  ///< Before updating entities. (Ex: Initializing an empty frame/clearing the previous one)
            OnUpdate = 400,   ///< Update entities, this is the default system. (Ex: Physics simulation)
            OnValidate = 500, ///< Validate entities state after the update. (Ex: Collision detection)
            PostUpdate = 600, ///< After updating entities. (Ex: Collision resolution)
            PreStore = 700,   ///< Before storing entities. (Ex: Transform matrices computation)
            OnStore = 800,    ///< Store entities. (Ex: Rendering)
        };

        ///
        /// @brief A phase in the pipeline.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        class Phase {
          private:
            ///
            /// @brief Construct a new empty Phase object.
            ///
            /// @note This constructor is private, use the Pipeline::getPhase() method to get a phase.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-11-21)
            ///
            Phase(Pipeline &pipeline, PhaseId id) : _pipeline(pipeline), _begin(), _size(0), _id(id)
            {
            }

          public:
            ///
            /// @brief Run all systems in this phase.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-11-21)
            ///
            void run();

            ///
            /// @brief Get the number of systems in this phase.
            ///
            /// @return std::size_t The number of systems in this phase.
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-11-21)
            ///
            [[nodiscard]] constexpr std::size_t getSize() const noexcept
            {
                return _size;
            }

            ///
            /// @brief Get the phase ID.
            ///
            /// @return constexpr PhaseId The phase identifier.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-11-21)
            ///
            [[nodiscard]] constexpr PhaseId getId() const noexcept
            {
                return _id;
            }

            ///
            /// @brief Get the iterator to the first system in this phase.
            ///
            /// @return SystemIterator Iterator to the first system in this phase.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-11-21)
            ///
            [[nodiscard]] SystemIterator begin() const noexcept;

            ///
            /// @brief Get the iterator past the last system in this phase.
            ///
            /// @warning This does @b not represent a system in this phase.
            ///
            /// @return SystemIterator Iterator past the last system in this phase.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-11-21)
            ///
            [[nodiscard]] SystemIterator end() const noexcept;

            ///
            /// @brief Get the phase timer.
            ///
            /// @note The timer is used to control the execution of the phase. You can also use
            /// @ref ecstasy::ISystem "ISystem" scale timers.
            ///
            /// @warning Rate limiting both the phase and the systems will result in multiplied timers (not added).
            /// However interval timers will be cumulative.
            ///
            /// @return Timer& Reference to the system timer.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-11-22)
            ///
            [[nodiscard]] constexpr Timer &getTimer() noexcept
            {
                return _timer;
            }

            ///
            /// @brief Get the phase timer.
            ///
            /// @note The timer is used to control the execution of the phase. You can also use
            /// @ref ecstasy::ISystem "ISystem" scale timers.
            ///
            /// @warning Rate limiting both the phase and the systems will result in multiplied timers (not added).
            /// However interval timers will be cumulative.
            ///
            /// @return const Timer& Reference to the system timer.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-11-22)
            ///
            [[nodiscard]] constexpr const Timer &getTimer() const noexcept
            {
                return _timer;
            }

          private:
            /// @brief Owning pipeline.
            Pipeline &_pipeline;
            /// @brief Index of the first system in this phase.
            std::size_t _begin;
            /// @brief Number of systems in this phase.
            std::size_t _size;
            /// @brief Identifier of the phase.
            PhaseId _id;
            /// @brief Timer to control the execution of the phase.
            Timer _timer;

            ///
            /// @brief Get the index of the first system in this phase.
            ///
            /// @return constexpr std::size_t The index of the first system in this phase.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-11-21)
            ///
            [[nodiscard]] constexpr std::size_t begin_idx() const
            {
                return _begin;
            }

            ///
            /// @brief Get the index past the last system in this phase.
            ///
            /// @warning This index is not valid to access the last system.
            ///
            /// @return constexpr std::size_t The index past the last system in this phase.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-11-21)
            ///
            [[nodiscard]] constexpr std::size_t end_idx() const
            {
                return _begin + _size;
            }

            /// @brief Allow Pipeline to access the private members.
            friend Pipeline;
        };

        ///
        /// @brief Construct a new Pipeline owned by the given registry.
        ///
        /// @param[in] registry The registry owning this pipeline.
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        Pipeline(Registry &registry);

        ///
        /// @brief Get a Phase instance from its identifier. Create it if it does not exist.
        ///
        /// @param[in] id Identifier of the phase.
        /// @param[in] autoRegister Whether to register the phase if it does not exist. Default is @c true.
        ///
        /// @return Phase& Reference to the phase.
        ///
        /// @throw std::out_of_range If the phase does not exist and @p autoRegister is @c false.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        [[nodiscard]] Phase &getPhase(PhaseId id, bool autoRegister = true);

        ///
        /// @brief Get the phase containing the given system.
        ///
        /// @param[in] system Type of the system to find.
        ///
        /// @return Phase& Reference to the phase containing the system.
        ///
        /// @throw std::out_of_range If the system is not in any phase.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        [[nodiscard]] const Phase &getSystemPhase(std::type_index system) const;

        ///
        /// @brief Get the phase containing the given system.
        ///
        /// @tparam S Type of the system to find.
        ///
        /// @return const Phase& Reference to the phase containing the system.
        ///
        /// @throw std::out_of_range If the system is not in any phase.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        template <std::derived_from<ISystem> S>
        [[nodiscard]] const Phase &getSystemPhase() const
        {
            return getSystemPhase(typeid(S));
        }

        ///
        /// @brief Register a phase with the given identifier.
        ///
        /// @note Does nothing if the phase already exists.
        ///
        /// @param[in] id Identifier of the phase.
        ///
        /// @return Phase& Reference to the newly created phase.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        Phase &registerPhase(PhaseId id);

        ///
        /// @brief Add a system to the pipeline.
        ///
        /// @note The systems are owned by the registry, not the pipeline.
        ///
        /// @param[in] system Type of the system to add.
        /// @param[in] phase Phase in which the system should be. Default is @ref Pipeline::PredefinedPhases::OnUpdate
        /// "OnUpdate".
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        void addSystem(std::type_index system, PhaseId phase = static_cast<std::size_t>(PredefinedPhases::OnUpdate));

        ///
        /// @brief Run a frame of the pipeline.
        ///
        /// @note This will run all phases by ascending order of their identifier.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        void run();

        ///
        /// @brief Run a specific phase of the pipeline.
        ///
        /// @param[in] phase Identifier of the phase to run.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        void run(PhaseId phase);

      private:
        /// @brief Ordered list of systems.
        std::vector<std::type_index> _systemsIds;
        /// @brief Ordered map of phases
        std::map<PhaseId, Phase> _phases;
        /// @brief Owning registry.
        Registry &_registry;
    };
} // namespace ecstasy

#endif /* !ECSTASY_SYSTEM_PIPELINE_HPP_ */
