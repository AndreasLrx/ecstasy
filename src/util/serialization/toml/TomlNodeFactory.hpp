///
/// @file TomlNodeFactory.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Toml node factory declaration.
/// @version 1.0.0
/// @date 2022-12-12
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_SERIALIZATION_TOML_TOMLFACTORY_HPP_
#define UTIL_SERIALIZATION_TOML_TOMLFACTORY_HPP_

#include "util/serialization/INode.hpp"
#include "util/serialization/INodeFactory.hpp"
#include <toml++/toml.h>

namespace util::serialization
{
    ///
    /// @brief Toml Node factory singleton.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-12)
    ///
    class TomlNodeFactory : public INodeFactory {
      public:
        ///
        /// @brief Retrieve the @ref TomlNodeFactory singleton instance.
        ///
        /// @return TomlNodeFactory& singleton instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] static TomlNodeFactory &get() noexcept;

        ///
        /// @brief Create a node from a @ref toml::node object.
        ///
        /// @param[in] node Toml node.
        ///
        /// @return NodePtr @ref TomlNode pointer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] NodePtr createFromToml(const toml::node &node);

        /// @copydoc INodeFactory::create(INode::Type).
        [[nodiscard]] NodePtr create(INode::Type type) override final;

        /// @copydoc INodeFactory::create(const INode &).
        [[nodiscard]] NodePtr create(const INode &node) override final;

        /// @copydoc INodeFactory::createObject().
        [[nodiscard]] NodePtr createObject(const IObjectNode &object) override final;

        /// @copydoc INodeFactory::createArray().
        [[nodiscard]] NodePtr createArray(const IArrayNode &array) override final;

        /// @copydoc INodeFactory::create(std::string_view).
        [[nodiscard]] NodePtr create(std::string_view string) override final;

        /// @copydoc INodeFactory::create(int64_t).
        [[nodiscard]] NodePtr create(int64_t integer) override final;

        /// @copydoc INodeFactory::create(double).
        [[nodiscard]] NodePtr create(double floatingPoint) override final;

        /// @copydoc INodeFactory::create(bool).
        [[nodiscard]] NodePtr create(bool boolean) override final;

        /// @copydoc INodeFactory::create(INode::Date).
        [[nodiscard]] NodePtr create(INode::Date date) override final;

        /// @copydoc INodeFactory::create(INode::Time).
        [[nodiscard]] NodePtr create(INode::Time time) override final;

        /// @copydoc INodeFactory::create(INode::DateTime).
        [[nodiscard]] NodePtr create(INode::DateTime dateTime) override final;

      private:
        /// @internal
        /// @brief Construct a new Toml Node Factory
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        TomlNodeFactory() = default;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLFACTORY_HPP_ */
