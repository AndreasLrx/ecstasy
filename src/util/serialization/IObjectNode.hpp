///
/// @file IObjectNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Object node interface.
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_SERIALIZATION_IOBJECTNODE_HPP_
#define UTIL_SERIALIZATION_IOBJECTNODE_HPP_

#include "INode.hpp"
#include "PolymorphicIterator.hpp"

namespace util::serialization
{
    ///
    /// @brief Object node. Contains nodes identified by strings.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class IObjectNode {
      protected:
        ///
        /// @brief Abstract Array Iterator.
        ///
        /// @tparam isConst Whether the iterator is a const iterator or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        template <bool isConst>
        using ObjectIterator =
            PolymorphicIterator<std::pair<std::string, std::conditional_t<isConst, NodeCView, NodeView>>>;

      public:
        /// @brief Object iterator type.
        using iterator = ObjectIterator<false>;
        /// @brief Object const iterator type.
        using const_iterator = ObjectIterator<true>;

        /// @brief Default destructor.
        virtual ~IObjectNode() = default;

        ///
        /// @brief Get the node matching @p key if existing.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return NodeCView A weak pointer to the requested node.
        ///
        /// @throw std::out_of_range If the key doesn't exists.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual NodeCView get(std::string_view key) const = 0;

        ///
        /// @brief Get the node matching @p key if existing.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return NodeView A weak pointer to the requested node.
        ///
        /// @throw std::out_of_range If the key doesn't exists.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual NodeView get(std::string_view key) = 0;

        ///
        /// @brief Get the node matching @p key.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return NodeCView A weak pointer to the requested node if it exists.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual NodeCView tryGet(std::string_view key) const noexcept = 0;

        ///
        /// @brief Get the node matching @p key.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return NodeView A weak pointer to the requested node if it exists.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual NodeView tryGet(std::string_view key) noexcept = 0;

        ///
        /// @brief Try to insert a new node at @p key.
        ///
        /// @warning If the key is already used, this method will fail. Use @ref insertOrAssign() instead.
        ///
        /// @param[in] key New node key.
        /// @param[in] value New node.
        ///
        /// @return bool Whether the node was inserted or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool insert(std::string_view key, const INode &value) = 0;

        ///
        /// @brief Try to insert a new node at @p key or replace the existing one.
        ///
        /// @param[in] key New node key.
        /// @param[in] value New node.
        ///
        /// @return bool Whether the node was inserted. It is false if it was assigned.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool insertOrAssign(std::string_view key, const INode &value) = 0;

        ///
        /// @brief Erase the node identified by @p key.
        ///
        /// @param[in] key Node's key.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void erase(std::string_view key) = 0;

        ///
        /// @brief Remove all the internal nodes.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void clear() = 0;

        ///
        /// @brief Check if the object is empty.
        ///
        /// @return bool True if @p this doesn't contains any node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool empty() const noexcept = 0;

        ///
        /// @brief Get the number of node in @p this.
        ///
        /// @return size_t Number of node contained in @p this.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual size_t size() const noexcept = 0;

        ///
        /// @brief Check if @p key match a node.
        ///
        /// @param[in] key Evaluated key.
        ///
        /// @return bool Whether there is a node associated to @p key.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool contains(std::string_view key) const noexcept = 0;

        ///
        /// @brief Get the start iterator of the internal nodes.
        ///
        /// @return const_iterator Iterator to the first node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual const_iterator cbegin() const noexcept = 0;

        /// @copydoc cbegin().
        [[nodiscard]] virtual const_iterator begin() const noexcept = 0;

        ///
        /// @brief Get the start iterator of the internal nodes.
        ///
        /// @return iterator Iterator to the first node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual iterator begin() noexcept = 0;

        ///
        /// @brief Get the end iterator of the internal nodes.
        ///
        /// @warning This iterator is @b never valid.
        ///
        /// @return const_iterator Iterator to the last node + 1.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual const_iterator cend() const noexcept = 0;

        /// @copydoc cend().
        [[nodiscard]] virtual const_iterator end() const noexcept = 0;

        ///
        /// @brief Get the end iterator of the internal nodes.
        ///
        /// @warning This iterator is @b never valid.
        ///
        /// @return iterator Iterator to the last node + 1.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual iterator end() noexcept = 0;
    };
} // namespace util::serialization
#endif /* !UTIL_SERIALIZATION_IOBJECTNODE_HPP_ */
