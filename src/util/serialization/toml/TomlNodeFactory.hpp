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

        /// @copydoc INodeFactory::exportStream().
        void exportStream(const INode &node, std::ostream &stream) override final;

        /// @copydoc INodeFactory::fromStream().
        NodePtr fromStream(std::istream &stream) override final;

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

        /// @copydoc INodeFactory::create().
        [[nodiscard]] NodePtr create(INode::Type type) override final;

        /// @copydoc INodeFactory::create().
        [[nodiscard]] NodePtr create(const INode &node) override final;

        /// @copydoc INodeFactory::createObject().
        [[nodiscard]] NodePtr createObject(const IObjectNode &object) override final;

        /// @copydoc INodeFactory::createArray().
        [[nodiscard]] NodePtr createArray(const IArrayNode &array) override final;

        /// @copydoc INodeFactory::create().
        [[nodiscard]] NodePtr create(std::string_view string) override final;

        /// @copydoc INodeFactory::create().
        [[nodiscard]] NodePtr create(int64_t integer) override final;

        /// @copydoc INodeFactory::create().
        [[nodiscard]] NodePtr create(double floatingPoint) override final;

        /// @copydoc INodeFactory::create().
        [[nodiscard]] NodePtr create(bool boolean) override final;

        /// @copydoc INodeFactory::create().
        [[nodiscard]] NodePtr create(INode::Date date) override final;

        /// @copydoc INodeFactory::create().
        [[nodiscard]] NodePtr create(INode::Time time) override final;

        /// @copydoc INodeFactory::create().
        [[nodiscard]] NodePtr create(INode::DateTime dateTime) override final;

      private:
        /// @internal
        /// @brief Construct a new Toml Node Factory
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        TomlNodeFactory() = default;

        /// @internal
        /// @brief Create a Toml Node object from a @ref INode object.
        ///
        /// @note This method is temporary and will be removed in the future, after a refactor of the INode.
        ///
        /// @param[in] node INode object.
        ///
        /// @return std::unique_ptr<toml::node> Toml node object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-21)
        ///
        [[nodiscard]] std::unique_ptr<toml::node> createTomlNode(const util::serialization::INode &node);
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLFACTORY_HPP_ */
