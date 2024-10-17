///
/// @file And.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Binary query modifier which performs a and between at least two queryables.
/// @version 1.0.0
/// @date 2022-10-27
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_MODIFIERS_AND_HPP_
#define ECSTASY_QUERY_MODIFIERS_AND_HPP_

#include "BinaryModifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"

namespace ecstasy::query::modifier
{
    ///
    /// @brief Binary query modifier which performs a and between at least two queryables.
    ///
    /// @note The mask is the result of the operation: Q1 & Q2 & Qs...
    ///
    /// @tparam AutoLock Lock the @ref Lockable queryables if true.
    /// @tparam Q1 Left operand queryable type.
    /// @tparam Q2 Right operand queryable type.
    /// @tparam Qs Additional operand queryable types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-27)
    ///
    template <bool AutoLock, Queryable Q1, Queryable Q2, Queryable... Qs>
    class AndImpl
        : public BinaryModifier<AndImpl<AutoLock, Q1, Q2, Qs...>, AutoLock, std::type_identity, Q1, Q2, Qs...> {
      private:
        /// @brief Helper type for the base class.
        using ModifierClass =
            BinaryModifier<AndImpl<AutoLock, Q1, Q2, Qs...>, AutoLock, std::type_identity, Q1, Q2, Qs...>;

      public:
        ///
        /// @brief Construct a new And Queryable modifier.
        ///
        /// @param[in] firstOperand left queryable operand.
        /// @param[in] secondOperand right queryable operand.
        /// @param[in] otherOperands additional operands (optional).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        AndImpl(Q1 &firstOperand, Q2 &secondOperand, Qs &...otherOperands)
            : ModifierClass(firstOperand, secondOperand, otherOperands...)
        {
        }

        ///
        /// @brief Get the specified operand data at the given index.
        ///
        /// @warning May throw exceptions, look at the specified operand type equivalent method documentation.
        ///
        /// @tparam operandId Id of the operand (where 0 is Q1, 1 is Q2...).
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return auto The operand data at index @p index.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        template <size_t operandId>
        [[nodiscard]] inline typename ModifierClass::template OperandData<operandId> getOperandData(size_t index)
        {
            return getQueryableData(std::get<operandId>(this->_operands), index);
        }

        ///
        /// @brief Update the mask to be up to date with the operands bitmasks.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        inline void reloadMask()
        {
            if constexpr (sizeof...(Qs) > 0)
                combineOperandMasks(std::make_index_sequence<(sizeof...(Qs))>());
            else
                this->_mask = ecstasy::query::getQueryableMask(std::get<0>(this->_operands))
                    & ecstasy::query::getQueryableMask(std::get<1>(this->_operands));
        }

      private:
        ///
        /// @brief Combine all the operands' bitmasks
        ///
        /// @tparam ints Sequence values.
        ///
        /// @param[in] int_seq Sequence containing the queryable ids.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-21)
        ///
        template <size_t... ints>
        inline void combineOperandMasks([[maybe_unused]] std::integer_sequence<size_t, ints...> int_seq)
        {
            this->_mask = ((util::BitSet(getQueryableMask(std::get<0>(this->_operands)))
                               & getQueryableMask(std::get<1>(this->_operands))) &= ... &=
                getQueryableMask(std::get<ints + 2>(this->_operands)));
        }
    };

    ///
    /// @brief Binary query modifier which performs a and between at least two queryables.
    ///
    /// @tparam Q1 Left operand queryable type.
    /// @tparam Q2 Right operand queryable type.
    /// @tparam Qs Additional operand queryable types.
    ///
    /// @param[in] firstOperand left queryable operand.
    /// @param[in] secondOperand right queryable operand.
    /// @param[in] otherOperands additional operands (optional).
    ///
    /// @return auto The resulting queryable modifier.
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <Queryable Q1, Queryable Q2, Queryable... Qs>
    [[nodiscard]] auto constexpr And(Q1 &firstOperand, Q2 &secondOperand, Qs &...otherOperands)
    {
        return AndImpl<false, Q1, Q2, Qs...>(firstOperand, secondOperand, otherOperands...);
    }
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIERS_AND_HPP_ */
