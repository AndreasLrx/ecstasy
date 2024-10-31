///
/// @file Modifier.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Type erased base class for all query modifiers.
/// @version 1.0.0
/// @date 2022-10-24
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_MODIFIER_MODIFIER_HPP_
#define ECSTASY_QUERY_MODIFIER_MODIFIER_HPP_

namespace ecstasy::query::modifier
{
    ///
    /// @brief Type erased base class for all query modifiers.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    class ModifierBase {
      public:
        ///
        /// @brief Destroy the modifier base object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        virtual ~ModifierBase() = default;
    };

    ///
    /// @brief Base class for all query modifiers.
    ///
    /// @tparam AutoLock Lock the @ref ecstasy::thread::Lockable "Lockable" queryables if true.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <bool AutoLock>
    class Modifier : public ModifierBase {
      public:
        /// @copydoc ModifierBase::~ModifierBase()
        virtual ~Modifier() = default;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIER_HPP_ */
