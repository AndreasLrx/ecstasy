///
/// @file PolymorphicIterator.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-13
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_SERIALIZATION_POLYMORPHICITERATOR_HPP_
#define UTIL_SERIALIZATION_POLYMORPHICITERATOR_HPP_

#include <memory>

namespace util::serialization
{
    ///
    /// @brief Polymorphism iterator for value T.
    ///
    /// @tparam T Value type of the iterator.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-13)
    ///
    template <typename T>
    class PolymorphicIterator {
      public:
        using value_type = T;
        using reference = value_type &;
        using pointer = value_type *;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

      private:
        /// @internal
        /// @brief Concept for the poymorphic iterators proxy.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        struct Concept {
            /// @brief Default destructor.
            virtual ~Concept() = default;

            ///
            /// @brief Increment the iterator by one.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            virtual void nextInplace() = 0;

            ///
            /// @brief Retrieve the iterator value.
            ///
            /// @return const value_type value associated with the iterator.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            virtual value_type get() const = 0;

            ///
            /// @brief Compare two operators.
            ///
            /// @warning The iterators must be issued from the same container.
            ///
            /// @param[in] other Other iterator.
            ///
            /// @return bool Whether the two iterators are equals.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            virtual bool equal(const Concept *other) const = 0;

            ///
            /// @brief Clone @b this.
            ///
            /// @return std::unique_ptr<Concept> Copy of @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            virtual std::unique_ptr<Concept> clone() const = 0;

            ///
            /// @brief Get the current iterator type info.
            ///
            /// @return const std::type_info& @b this type info.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            virtual const std::type_info &type() const = 0;

            ///
            /// @brief Get @b this address.
            ///
            /// @return const Concept* @b this address.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            virtual const Concept *address() const = 0;
        };

        /// @internal
        /// @brief Instance of the @ref Concept.
        ///
        /// @tparam Iter Wrapped iterator type.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        template <class Iter>
        class Model : public Concept {
          public:
            ///
            /// @brief Construct a new Model.
            ///
            /// @param[in] iter iterator to wrap.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-13)
            ///
            Model(Iter iter) : _iter(iter)
            {
            }

            /// @copydoc Concept::nextInplace()
            void nextInplace() override final
            {
                ++_iter;
            }

            /// @copydoc Concept::get()
            value_type get() const override final
            {
                return *_iter;
            }

            /// @copydoc Concept::equal()
            bool equal(const Concept *rp) const override final
            {
                return _iter == dynamic_cast<const Model *>(rp)->_iter;
            }

            /// @copydoc Concept::clone()
            std::unique_ptr<Concept> clone() const override final
            {
                return std::make_unique<Model>(*this);
            }

            /// @copydoc Concept::type()
            const std::type_info &type() const override final
            {
                return typeid(_iter);
            }

            /// @copydoc Concept::address()
            const Concept *address() const override final
            {
                return this;
            }

          private:
            Iter _iter;
        };

      public:
        ///
        /// @brief Construct a new Polymorphic Iterator.
        ///
        /// @tparam Iter Internal iterator type.
        ///
        /// @param[in] iter Iterator type.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        template <class Iter>
        PolymorphicIterator(Iter iter) : _impl(std::make_unique<Model<Iter>>(iter))
        {
        }

        ///
        /// @brief Construct a copy of an existing Polymorphic Iterator
        ///
        /// @param[in] r Iterator to copy.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        PolymorphicIterator(const PolymorphicIterator &r) : _impl(r._impl->clone())
        {
        }

        ///
        /// @brief Fetch the iterator value.
        ///
        /// @return const value_type Iterator value.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        value_type operator*() const
        {
            return _impl->get();
        }

        ///
        /// @brief Increment the internal iteraor inplace.
        ///
        /// @return PolymorphicIterator& @b this.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        PolymorphicIterator &operator++()
        {
            _impl->nextInplace();
            return *this;
        }

        ///
        /// @brief Copy @b this and increment it.
        ///
        /// @warning You should use the inplace increment instead.
        ///
        /// @return PolymorphicIterator Incremented copy of @b this.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        PolymorphicIterator operator++(int)
        {
            PolymorphicIterator res = *this;

            return ++res;
        }

        ///
        /// @brief Equality operator.
        ///
        /// @param[in] r Other iterator.
        ///
        /// @return bool Whether the two iterators are equal.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        bool operator==(const PolymorphicIterator &r) const
        {
            return _impl->type() == r._impl->type() and _impl->equal(r._impl->address());
        }

        ///
        /// @brief Inequality operator.
        ///
        /// @param[in] r Other iterator.
        ///
        /// @return bool Whether the two iterators are not equal.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        bool operator!=(const PolymorphicIterator &r) const
        {
            return !(*this == r);
        }

      private:
        std::unique_ptr<Concept> _impl;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_POLYMORPHICITERATOR_HPP_ */
