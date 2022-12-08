///
/// @file IObjectNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_IOBJECTNODE_HPP_
#define UTIL_SERIALIZATION_IOBJECTNODE_HPP_

#include "INode.hpp"

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
        /// @brief Abstract Object Iterator.
        ///
        /// @tparam isConst Whether the iterator is a const iterator or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        template <bool isConst>
        class ObjectIterator {
          public:
            using value_type = std::conditional_t<isConst, const INode &, INode &>;
            using reference = value_type &;
            using pointer = value_type *;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::random_access_iterator_tag;

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            // All iterators must be constructible, copy-constructible, copy-assignable, destructible and swappable.///
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Construct a new uninitialized ObjectIterator.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            explicit ObjectIterator()
            {
            }

            ///
            /// @brief Default copy operator.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            ObjectIterator(ObjectIterator const &) = default;

            ///
            /// @brief Default assignment operator.
            ///
            /// @return ObjectIterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            ObjectIterator &operator=(ObjectIterator const &) = default;

            ///
            /// @brief Default move constructor.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            ObjectIterator(ObjectIterator &&) = default;

            ///
            /// @brief Default move assignment operator.
            ///
            /// @return ObjectIterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            ObjectIterator &operator=(ObjectIterator &&) = default;

            ///
            /// @brief Compare two iterators from the same @ref IObjectNode.
            ///
            /// @warning It is undefined behavior to compare two iterators that do not belong to the same object.
            ///
            /// @param[in] other iterator to compare.
            ///
            /// @return bool Whether the two iterators are equals.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            virtual bool operator==(ObjectIterator const &other) const = 0;

            ///
            /// @brief Compare two iterators from the same @ref IObjectNode.
            ///
            /// @warning It is undefined behavior to compare two iterators that do not belong to the same object.
            ///
            /// @param[in] other iterator to compare.
            ///
            /// @return bool Whether the two iterators are different.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            virtual bool operator!=(ObjectIterator const &other) const = 0;

            ///
            /// @brief Fetch the node at the current position.
            ///
            /// @return @ref value_type Node.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            virtual value_type operator*() const = 0;

            ///
            /// @brief Increments the iterator in place.
            ///
            /// @warning It is undefined behavior to increment the iterator past the end sentinel ( @ref
            /// IObjectNode::end() ).
            ///
            /// @return ObjectIterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            virtual ObjectIterator &operator++() = 0;

            ///
            /// @brief Copies the iterator and increments the copy, please use pre-incrementation instead.
            ///
            /// @warning It is undefined behavior to increment the iterator past the end sentinel ( @ref
            /// IObjectNode::end() ).
            /// @warning This creates a copy of the iterator!
            ///
            /// @return ObjectIterator The incremented copy.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            virtual ObjectIterator operator++(int) = 0;

            ///
            /// @brief Fetch the node at the current position.
            ///
            /// @return @ref value_type Node.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            value_type operator->() const
            {
                return *this;
            }
        };

      public:
        /// @brief Array iterator type.
        using iterator = ObjectIterator<false>;
        /// @brief Array const iterator type.
        using const_iterator = ObjectIterator<true>;

        /// @brief Default destructor.
        virtual ~IObjectNode() = default;

        ///
        /// @brief Get the node matching @p key if existing.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return const INode& A const reference to the requested node.
        ///
        /// @throw std::out_of_range If the key doesn't exists.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual const INode &get(std::string_view key) const = 0;

        ///
        /// @brief Get the node matching @p key if existing.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return INode& A reference to the requested node.
        ///
        /// @throw std::out_of_range If the key doesn't exists.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual INode &get(std::string_view key) = 0;

        ///
        /// @brief Get the node matching @p key.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return std::optional<const INode&> A const reference to the requested node if it exists, an empty @ref
        /// std::optional otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<const INode &> tryGet(std::string_view key) const = 0;

        ///
        /// @brief Get the node matching @p key.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return std::optional<INode&> A reference to the requested node if it exists, an empty @ref
        /// std::optional otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<INode &> tryGet(std::string_view key) = 0;

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
        /// @return bool Whether a node was erased or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool erase(std::string_view key) = 0;

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
        virtual bool empty() const = 0;

        ///
        /// @brief Get the number of node in @p this.
        ///
        /// @return size_t Number of node contained in @p this.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual size_t size() const = 0;

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
        virtual bool contains(std::string_view key) const = 0;

        ///
        /// @brief Get the start iterator of the internal nodes.
        ///
        /// @return const_iterator Iterator to the first node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual const_iterator cbegin() const = 0;

        ///
        /// @brief Get the start iterator of the internal nodes.
        ///
        /// @return iterator Iterator to the first node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual iterator begin() = 0;

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
        virtual const_iterator cend() const = 0;

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
        virtual iterator end() = 0;
    };
} // namespace util::serialization
#endif /* !UTIL_SERIALIZATION_IOBJECTNODE_HPP_ */
