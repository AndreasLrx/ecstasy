///
/// @file TomlArrayNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_TOML_TOMLARRAYNODE_HPP_
#define UTIL_SERIALIZATION_TOML_TOMLARRAYNODE_HPP_

#include "TomlNode.hpp"
#include "util/serialization/IArrayNode.hpp"

namespace util::serialization
{
    ///
    /// @brief Toml Array node. Contains nodes in an array.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class TomlArrayNode : public TomlNode<toml::array>, IArrayNode {
      protected:
        ///
        /// @brief Abstract Array Iterator.
        ///
        /// @tparam isConst Whether the iterator is a const iterator or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        template <bool isConst = false>
        class Iterator {
          public:
            using value_type = std::conditional_t<isConst, NodeCView, NodeView>;
            using reference = value_type &;
            using pointer = value_type *;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::random_access_iterator_tag;
            using array_type = std::conditional_t<isConst, const TomlArrayNode, TomlArrayNode>;

            /// @brief Default constructor.
            Iterator() = default;

            ///
            /// @brief Construct a new Iterator.
            ///
            /// @param[in] array Owning array.
            /// @param[in] pos Current position.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            Iterator(array_type &array, Index pos = 0) : _pos(pos), _array(&array)
            {
            }

            ///
            /// @brief Default copy operator.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            Iterator(Iterator const &) = default;

            ///
            /// @brief Default assignment operator.
            ///
            /// @return Iterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            Iterator &operator=(Iterator const &) = default;

            ///
            /// @brief Default move constructor.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            Iterator(Iterator &&) = default;

            ///
            /// @brief Default move assignment operator.
            ///
            /// @return Iterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            Iterator &operator=(Iterator &&) = default;

            ///
            /// @brief Equality operator.
            ///
            /// @warning It is undefined behavior to compare two iterators which doesn't belong to the same container.
            ///
            /// @param[in] other Other iterator.
            ///
            /// @return bool Whether the two iterators are equal.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            bool operator==(Iterator const &other) const
            {
                return _pos == other._pos;
            }

            ///
            /// @brief Inequality operator.
            ///
            /// @warning It is undefined behavior to compare two iterators which doesn't belong to the same container.
            ///
            /// @param[in] other Other iterator.
            ///
            /// @return bool Whether the two iterators are inequal.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            bool operator!=(Iterator const &other) const
            {
                return !(*this == other);
            }

            ///
            /// @brief Fetch the iterator value.
            ///
            /// @return value_type iterator value.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            value_type operator*() const
            {
                return _array->get(_pos);
            }

            ///
            /// @brief Prefix increment operator.
            ///
            /// @return Iterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            Iterator &operator++()
            {
                ++_pos;
                return *this;
            }

            ///
            /// @brief Suffix increment operator.
            ///
            /// @warning This create a copy of the iterator.
            ///
            /// @return Iterator Incremented copy of @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            Iterator operator++(int)
            {
                Iterator it = *this;

                return ++it;
            }

          private:
            Index _pos;
            array_type *_array;
        };

      public:
        /// @brief Default constructor.
        TomlArrayNode() = default;

        ///
        /// @brief Construct a new Toml Array Node.
        ///
        /// @param[in] array Toml array.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        TomlArrayNode(toml::array &array);

        /// @copydoc IArrayNode::&get()
        NodeCView get(Index index) const override final;

        /// @copydoc IArrayNode::&get()
        NodeView get(Index index) override final;

        /// @copydoc IArrayNode::tryGet()
        NodeCView tryGet(Index index) const override final;

        /// @copydoc IArrayNode::tryGet()
        NodeView tryGet(Index index) override final;

        /// @copydoc IArrayNode::pushBack()
        void pushBack(const INode &node) override final;

        /// @copydoc IArrayNode::insert()
        void insert(Index index, const INode &node) override final;

        /// @copydoc IArrayNode::replace()
        void replace(Index index, const INode &node) override final;

        /// @copydoc IArrayNode::popBack()
        void popBack() override final;

        /// @copydoc IArrayNode::erase()
        void erase(Index index) override final;

        /// @copydoc IArrayNode::clear()
        void clear() override final;

        /// @copydoc IArrayNode::empty()
        bool empty() const override final;

        /// @copydoc IArrayNode::size()
        size_t size() const override final;

        /// @copydoc IArrayNode::cbegin()
        const_iterator cbegin() const override final;

        /// @copydoc IArrayNode::begin()
        const_iterator begin() const override final;

        /// @copydoc IArrayNode::begin()
        iterator begin() override final;

        /// @copydoc IArrayNode::cend()
        const_iterator cend() const override final;

        /// @copydoc IArrayNode::end()
        const_iterator end() const override final;

        /// @copydoc IArrayNode::end()
        iterator end() override final;

      private:
        std::vector<NodePtr> _nodes;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLARRAYNODE_HPP_ */
