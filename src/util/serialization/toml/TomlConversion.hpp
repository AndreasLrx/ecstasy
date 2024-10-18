///
/// @file TomlConversion.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Toml conversion interface.
/// @version 1.0.0
/// @date 2022-12-12
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_SERIALIZATION_TOML_TOMLCONVERSION_HPP_
#define UTIL_SERIALIZATION_TOML_TOMLCONVERSION_HPP_

#include "util/serialization/INode.hpp"
#include <toml++/toml.h>

namespace util::serialization
{
    class TomlConversion {
      public:
        ///
        /// @brief Convert a @ref INode::Date to a @ref toml::date.
        ///
        /// @param[in] date common serializer date.
        ///
        /// @return toml::date Equivalent toml date.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        [[nodiscard]] static toml::date toToml(const INode::Date &date) noexcept;

        ///
        /// @brief Convert a @ref INode::Time to a @ref toml::time.
        ///
        /// @param[in] time common serializer time.
        ///
        /// @return toml::time Equivalent toml time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        [[nodiscard]] static toml::time toToml(const INode::Time &time) noexcept;

        ///
        /// @brief Convert a @ref INode::DateTime to a @ref toml::date_time.
        ///
        /// @param[in] dateTime common serializer date time.
        ///
        /// @return toml::date_time Equivalent toml date time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        [[nodiscard]] static toml::date_time toToml(const INode::DateTime &dateTime) noexcept;

        ///
        /// @brief Convert a @ref toml::date to a @ref INode::Date.
        ///
        /// @param[in] date toml date.
        ///
        /// @return INode::Date Equivalent serialize date.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        [[nodiscard]] static INode::Date fromToml(const toml::date &date) noexcept;

        ///
        /// @brief Convert a @ref toml::time to a @ref INode::Time.
        ///
        /// @param[in] time toml time.
        ///
        /// @return INode::Time Equivalent serializer time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        [[nodiscard]] static INode::Time fromToml(const toml::time &time) noexcept;

        ///
        /// @brief Convert a @ref toml::date_time to a @ref INode::DateTime.
        ///
        /// @param[in] dateTime toml date time.
        ///
        /// @return INode::DateTime Equivalent serializer date time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        [[nodiscard]] static INode::DateTime fromToml(const toml::date_time &dateTime) noexcept;

      private:
        TomlConversion() = default;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLCONVERSION_HPP_ */
