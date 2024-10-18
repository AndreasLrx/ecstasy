///
/// @file IArrayNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Array node interface.
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_SERIALIZATION_IARRAYNODE_HPP_
#define UTIL_SERIALIZATION_IARRAYNODE_HPP_

#include <memory>
#include "INode.hpp"
#include "PolymorphicIterator.hpp"

namespace util::serialization
{
    ///
    /// @brief Array node. Contains nodes in an array.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class IArrayNode {
      protected:
        ///
        /// @brief Array Iterator.
        ///
        /// @tparam isConst Whether the iterator is a const iterator or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        template <bool isConst>
        using ArrayIterator = PolymorphicIterator<std::conditional_t<isConst, NodeCView, NodeView>>;

      public:
        /// @brief Array index type.
        using Index = size_t;
        /// @brief Array iterator type.
        using iterator = ArrayIterator<false>;
        /// @brief Array const iterator type.
        using const_iterator = ArrayIterator<true>;

        /// @brief Default destructor
        virtual ~IArrayNode() = default;

        ///
        /// @brief Get the node at @p index if existing.
        ///
        /// @param[in] index The node's index.
        ///
        /// @return NodeCView A weak pointer to the requested node.
        ///
        /// @throw std::out_of_range If the index is out of bounds.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual NodeCView get(Index index) const = 0;

        ///
        /// @brief Get the node at @p index if existing.
        ///
        /// @param[in] index The node's index.
        ///
        /// @return NodeView A weak pointer to the requested node.
        ///
        /// @throw std::out_of_range If the index is out of bounds.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual NodeView get(Index index) = 0;

        ///
        /// @brief Get the node at @p index.
        ///
        /// @param[in] index The node's index.
        ///
        /// @return NodeCView A weak pointer to the requested node if it exists.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual NodeCView tryGet(Index index) const noexcept = 0;

        ///
        /// @brief Get the node at @p index.
        ///
        /// @param[in] index The node's index.
        ///
        /// @return NodeCView A weak pointer to the requested node if it exists.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual NodeView tryGet(Index index) noexcept = 0;

        ///
        /// @brief Push a new node at the end of the array.
        ///
        /// @param[in] node Node to push.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void pushBack(const INode &node) = 0;

        ///
        /// @brief Insert a node at the given index.
        ///
        /// @note All elements after (greater index) @p index will be moved one index further.
        ///
        /// @param[in] index Index of the new node.
        /// @param[in] node Node to insert.
        ///
        /// @throw std::out_of_range If index is @b greater than the current @ref size().
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void insert(Index index, const INode &node) = 0;

        ///
        /// @brief Replace a node at the given index.
        ///
        /// @note Other indexes are not changed.
        ///
        /// @param[in] index Index of the node.
        /// @param[in] node New node value.
        ///
        /// @throw std::out_of_range If @p index is @b greater than or @b equal the current @ref size().
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void replace(Index index, const INode &node) = 0;

        ///
        /// @brief Delete the last array node.
        ///
        /// @note Does nothing if the array is @ref empty().
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void popBack() = 0;

        ///
        /// @brief Delete the node at @p index.
        ///
        /// @param[in] index Index of the node to erase.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void erase(Index index) = 0;

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

#endif /* !UTIL_SERIALIZATION_IARRAYNODE_HPP_ */
