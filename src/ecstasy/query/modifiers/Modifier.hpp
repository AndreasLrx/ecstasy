///
/// @file Modifier.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
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
    /// @brief Base class for all query modifiers.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    class Modifier {
      public:
        virtual ~Modifier() = default;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIER_HPP_ */
