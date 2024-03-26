///
/// @file StackAllocator.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Stack Allocator
/// @version 1.0.0
/// @date 2023-11-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_STACKALLOCATOR_HPP_
#define UTIL_STACKALLOCATOR_HPP_

#include <cstring>
#include <memory>
#include <new>
#include <vector>

namespace util
{
    /// @internal
    /// @brief Template of a vector to manage lifetime of multiple instances inheriting the same base class.
    /// This allocator allocate instances on the stack and therefore requires to know the exact byte size at compile
    /// time (or expect uncatchable program segmentation fault or memory corruption).
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <size_t size, typename Base>
    class StackAllocator {
      public:
        /// @brief Size in byte of the allocator memory
        ///
        /// @note This is an alias to the template parameter @tparam size
        static constexpr size_t mem_size = size;

        ///
        /// @brief Default constructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2023-11-08)
        ///
        StackAllocator()
        {
            _cursor = 0;
        }

        ///
        /// @brief Move constructor.
        ///
        /// @warning Instances must not have pointers/reference to object in @p other memory block.
        ///
        /// @param[in] other Existing allocator to copy.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2023-11-08)
        ///
        StackAllocator(StackAllocator &&other)
        {
            *this = std::move(other);
        }

        ///
        /// @brief Destructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2023-11-08)
        ///
        ~StackAllocator()
        {
            for (auto &&object : _instances)
                (*object).~Base();
        }

        ///
        /// @brief Move assignment operator.
        ///
        /// @warning Instances must not have pointers/reference to object in @p other memory block.
        ///
        /// @param[in] other Existing allocator to move into @b this.
        ///
        /// @return StackAllocator& @b this
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2023-11-08)
        ///
        StackAllocator &operator=(StackAllocator &&other)
        {
            // Doesn't support overlap but we shouldn't have any
            std::memcpy(_memory, other._memory, size);
            _cursor = other._cursor;
            for (Base *instance : other._instances)
                _instances.push_back(reinterpret_cast<Base *>(reinterpret_cast<char *>(_memory)
                    + (reinterpret_cast<char *>(instance) - reinterpret_cast<char *>(other._memory))));

            // Clear other
            other._cursor = 0;
            other._instances.clear();
            return *this;
        }

        ///
        /// @brief Instanciate a new instance of type T with lifetime attached to @b this lifetime.
        ///
        /// @tparam T Type of the new object.
        /// @tparam Args Arguments Types of the object constructor.
        ///
        /// @param[in] args Arguments to forward to the object constructor.
        ///
        /// @return T& newly created object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        template <std::derived_from<Base> T, typename... Args>
        T &instanciate(Args &&...args)
        {
            T *newObject = new (reinterpret_cast<void *>(&_memory[_cursor])) T(std::forward<Args>(args)...);

            _instances.push_back(newObject);
            _cursor += sizeof(T);
            return *newObject;
        }

        ///
        /// @brief Access the internal instances vector.
        ///
        /// @return const std::vector<Base *>& Read only instances vector.
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2023-11-08)
        ///
        constexpr const std::vector<Base *> &getInstances() const
        {
            return _instances;
        }

      private:
        char _memory[size];

        size_t _cursor;
        std::vector<Base *> _instances;
    };
} // namespace util

#endif /* !UTIL_STACKALLOCATOR_HPP_ */
