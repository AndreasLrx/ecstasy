///
/// @file ITomlNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Toml node interface.
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_SERIALIZATION_TOML_ITOMLNODE_HPP_
#define UTIL_SERIALIZATION_TOML_ITOMLNODE_HPP_

#include "util/serialization/ANode.hpp"
#include <toml++/toml.h>

namespace util::serialization
{

    ///
    /// @brief Toml node interface.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-21)
    ///
    class ITomlNode : public ANode {
      public:
        ///
        /// @brief Destroy the ITomlNode instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-21)
        ///
        virtual ~ITomlNode() = default;

        ///
        /// @brief Get a const reference to the internal toml node.
        ///
        /// @return const toml::node& Internal toml node const reference.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-21)
        ///
        [[nodiscard]] virtual const toml::node &getNode() const noexcept = 0;

        ///
        /// @brief Get a reference to the internal toml node.
        ///
        /// @return toml::node& Internal toml node reference.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-21)
        ///
        [[nodiscard]] virtual toml::node &getNode() noexcept = 0;
    };

} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_ITOMLNODE_HPP_ */
