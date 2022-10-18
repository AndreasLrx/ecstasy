///
/// @file Bitset.hpp
/// @author Yanis Guaye (yanis.guaye@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-18
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_BITSET_HPP_
#define UTIL_BITSET_HPP_

#include <bit>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <vector>
#include <string_view>

// As of 2022, C++20 is still considered "experimental" by G++, `constexpr` for std::vector was only added in G++ 12.1.
// So we need to disable `constexpr` on most BitSet functions in G++ versions below 12.1.
//
// For G++ 12.1 and above, em++, and MSVC, constexpr support is enabled.
#if !defined(__clang__) && defined(__GNUG__) && (__GNUG__ < 12 || __GNUC_MINOR__ < 1)
    #define BIT_SET_CONSTEXPR inline
#else
    #define BIT_SET_CONSTEXPR constexpr
#endif // !defined(__clang__) && defined(__GNUG__) && (__GNUG__ < 12 || __GNUC_MINOR__ < 1)

namespace util
{
    /// Mimics the API of std::bitset but with the dynamic properties of std::vector<bool>
    class BitSet {
      public:
        class Reference {
          public:
            /// Assigns a value to the refenced bit.
            BIT_SET_CONSTEXPR Reference &operator=(bool value) noexcept
            {
                this->_set.set(this->_pos, value);
                return *this;
            }

            /// @returns The value of the referenced bit.
            BIT_SET_CONSTEXPR operator bool() const noexcept
            {
                return const_cast<BitSet const &>(this->_set)[this->_pos];
            }

            /// @returns The inverse of the referenced bit.
            BIT_SET_CONSTEXPR bool operator~() const noexcept
            {
                return !bool(*this);
            }

            /// Inverts the referenced bit.
            BIT_SET_CONSTEXPR Reference &flip() noexcept
            {
                *this = ~(*this);
                return *this;
            }

          private:
            BitSet &_set;
            std::size_t _pos;

            constexpr Reference(BitSet &set, std::size_t pos) : _set(set), _pos(pos)
            {
            }

            friend BitSet;
        };

        /// @param initialSize An initial number of bits, all set to zero.
        BitSet(std::size_t initialSize = 0);

        /// Constructs a bitset from a strings of zeroes and ones.
        ///
        /// The leftmost bit is the most-significant.
        ///
        /// @note @b bitString must @b only contain the chars '0' and '1'.
        BitSet(std::string_view bitString);

        /// @returns The number of bits in the set.
        constexpr std::size_t size() const noexcept
        {
            return this->_size;
        }

        /// @note This function does not perform bounds-checking.
        ///
        /// @returns The value of the bit at @b pos.
        BIT_SET_CONSTEXPR bool test(std::size_t pos) const noexcept
        {
            return (this->_store[pos >> 6] & mask(pos)) != 0;
        }

        /// @note This function does not perform bounds-checking.
        ///
        /// @returns The value of the bit at @b pos.
        BIT_SET_CONSTEXPR bool operator[](std::size_t pos) const noexcept
        {
            return this->test(pos);
        }

        /// @note This function does not perform bounds-checking.
        ///
        /// Assigns a value to the bit at @b pos.
        BIT_SET_CONSTEXPR BitSet &set(std::size_t pos, bool value = true) noexcept
        {
            if (value)
                this->_store[pos >> 6] |= mask(pos);
            else
                this->_store[pos >> 6] &= ~mask(pos);
            return *this;
        }

        /// Sets all bits to the given value.
        BitSet &setAll(bool value = true) noexcept;

        /// @note This function does not perform bounds-checking.
        ///
        /// @returns A reference to the bit at @b pos.
        constexpr Reference operator[](std::size_t pos) noexcept
        {
            return Reference(*this, pos);
        }

        /// @returns Whether the bit sets are equal, both sets must have the same size.
        bool operator==(BitSet const &other) const noexcept;

        /// @returns Whether the bit sets are not equal, both sets must have the same size.
        bool operator!=(BitSet const &other) const noexcept;

        /// Flips all the bits in the set.
        ///
        /// @returns this.
        BitSet &flip() noexcept;

        /// @returns A copy of this with all the bits flipped.
        BitSet operator~() const;

        /// Performs a bitwise AND with this set and @b other.
        ///
        /// @returns A new result set, its length is that of the smallest input set.
        BitSet operator&(BitSet const &other) const;

        /// Performs a bitwise OR with this set and @b other.
        ///
        /// @returns A new result set, its length is that of the smallest input set.
        BitSet operator|(BitSet const &other) const;

        /// Performs a bitwise XOR with this set and @b other.
        ///
        /// @returns A new result set, its length is that of the smallest input set.
        BitSet operator^(BitSet const &other) const;

        /// Performs a bitwise AND with this set and @b other in place.
        ///
        /// @returns this
        BitSet &operator&=(BitSet const &other) noexcept;

        /// Performs a bitwise OR with this set and @b other in place.
        ///
        /// @returns this
        BitSet &operator|=(BitSet const &other) noexcept;

        /// Performs a bitwise XOR with this set and @b other in place.
        ///
        /// @returns this
        BitSet &operator^=(BitSet const &other) noexcept;

        /// Changes the number of bits stored in this set.
        ///
        /// @param size The new number of bits, may be smaller or greater than the current size, or zero.
        inline void resize(std::size_t size)
        {
            this->_resize(size);
            this->normalize();
        }

        /// Changes the number of bits stored in this set, and sets the final bit past @b size to the value of
        /// @b sentinelValue.
        ///
        /// @param size The new number of non-sentinel bits, may be smaller or greater than the current size, or zero.
        /// @param sentinelValue The value of the sentinel bit.
        void resizeSentinel(std::size_t size, bool sentinelValue);

        /// Adds the given bit to the end of the set.
        BitSet &push(bool value);

        /// Removes one bit from the end of the set.
        ///
        /// @note This function does not perform bounds-checking.
        ///
        /// @returns The value of the removed bit.
        bool pop();

        /// Finds the first set bit in the set, starting from (and including) @b start.
        ///
        /// @note The behavior is undefined if the set does not contain a single 'one' bit from @b start.
        ///
        /// @param start The bit position of where to start the search for a set bit.
        ///
        /// @returns The position of the first set bit.
        BIT_SET_CONSTEXPR std::size_t firstSet(std::size_t start = 0) const
        {
            std::uint64_t mask = (~std::uint64_t(0)) << (start & 0b111111);
            std::size_t word_pos = start >> 6;
            std::uint64_t word = this->_store[word_pos] & mask;

            while (word == 0) {
                ++word_pos;
                word = this->_store[word_pos];
            }
            return (word_pos << 6) + std::countr_zero(word);
        }

        /// Finds the first unset bit in the set, starting from (and including) @b start.
        ///
        /// @note The behavior is undefined if the set does not contain a single 'zero' bit from @b start.
        ///
        /// @param start The bit position of where to start the search for an unset bit.
        ///
        /// @returns The position of the first unset bit.
        BIT_SET_CONSTEXPR std::size_t firstUnset(std::size_t start = 0) const
        {
            std::uint64_t mask = (~std::uint64_t(0)) << (start & 0b111111);
            std::size_t word_pos = start >> 6;
            std::uint64_t word = this->_store[word_pos] | (~mask);

            while (word == (~std::uint64_t(0))) {
                ++word_pos;
                word = this->_store[word_pos];
            }
            return (word_pos << 6) + std::countr_one(word);
        }

      private:
        std::size_t _size;
        std::vector<std::uint64_t> _store;

        /// @returns The bit mask for the requested bit position.
        static constexpr std::uint64_t mask(std::size_t pos)
        {
            return std::uint64_t(1) << (pos & 0b111111);
        }

        /// @returns The number of 64-bit words needed to store @b bitCount amount of bits.
        static constexpr std::size_t getStoreWordCount(std::size_t bitCount)
        {
            if ((bitCount & (~0b111111)) == bitCount)
                return bitCount >> 6; // if bitCount is a multiple of 64, return bitCount / 64
            else
                return (bitCount >> 6) + 1; // or else, add one word to contain the extra bits
        }

        /// Unset the trailing bits.
        void normalize() noexcept;

        /// Resize without normalizing
        void _resize(std::size_t size);

        friend Reference;
        /// Used by unit tests to access BitSet internals.
        friend class BitSetTester;
    };

    /// Prints the contents of @b set into @b output.
    std::ostream &operator<<(std::ostream &output, BitSet const &set);
} // namespace util

#endif // !defined(UTIL_BITSET_HPP_)
