///
/// @file And.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-27
///
/// @copyright Copyright (c) ECSTASY 2022
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
    /// @tparam Q1 Left operand queryable type.
    /// @tparam Q2 Right operand queryable type.
    /// @tparam Qs Additional operand queryable types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-27)
    ///
    template <Queryable Q1, Queryable Q2, Queryable... Qs>
    class And : public BinaryModifier<And<Q1, Q2, Qs...>, std::type_identity, Q1, Q2, Qs...> {
      private:
        /// @brief Helper type for the base class.
        using ModifierClass = BinaryModifier<And<Q1, Q2, Qs...>, std::type_identity, Q1, Q2, Qs...>;

      public:
        ///
        /// @brief Construct a new And Queryable modifier.
        ///
        /// @param[in] leftOperand left queryable operand.
        /// @param[in] rightOperand right queryable operand.
        /// @param[in] otherOperands additional operands (optional).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        And(Q1 &firstOperand, Q2 &secondOperand, Qs &...otherOperands)
            : ModifierClass(firstOperand, secondOperand, otherOperands...)
        {
        }

        ///
        /// @brief Get the specified operand data at the given index.
        ///
        /// @warning May throw exceptions, look at the specified operand type equivalent method documentation.
        ///
        /// @param[in] operandId Id of the operand (where 0 is Q1, 1 is Q2...).
        /// @param[in] index Index of the entity.
        ///
        /// @return auto The operand data at index @p index.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        template <size_t operandId>
        inline typename ModifierClass::OperandData<operandId> getOperandData(size_t index)
        {
            return std::get<operandId>(this->_operands).getQueryData(index);
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
                this->_mask = std::get<0>(this->_operands).getMask() & std::get<1>(this->_operands).getMask();
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
        inline void combineOperandMasks(std::integer_sequence<size_t, ints...> int_seq)
        {
            (void)int_seq;
            this->_mask =
                ((util::BitSet(std::get<0>(this->_operands).getMask()) & std::get<1>(this->_operands).getMask()) &=
                    ... &= std::get<ints + 2>(this->_operands).getMask());
        }
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIERS_AND_HPP_ */
