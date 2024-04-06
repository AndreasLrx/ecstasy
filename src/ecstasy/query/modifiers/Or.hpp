///
/// @file Or.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-27
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_MODIFIERS_OR_HPP_
#define ECSTASY_QUERY_MODIFIERS_OR_HPP_

#include "BinaryModifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "util/meta/add_optional.hpp"

namespace ecstasy::query::modifier
{
    ///
    /// @brief Binary query modifier which performs a or between at least two queryables.
    ///
    /// @note The mask is the result of the operation: Q1 | Q2 | Qs...
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
    class OrImpl
        : public BinaryModifier<OrImpl<AutoLock, Q1, Q2, Qs...>, AutoLock, util::meta::add_optional, Q1, Q2, Qs...> {
        /// @brief Helper type for the base class.
        using ModifierClass =
            BinaryModifier<OrImpl<AutoLock, Q1, Q2, Qs...>, AutoLock, util::meta::add_optional, Q1, Q2, Qs...>;

      public:
        ///
        /// @brief Construct a new Or Queryable modifier.
        ///
        /// @param[in] firstOperand left queryable operand.
        /// @param[in] secondOperand right queryable operand.
        /// @param[in] otherOperands additional operands (optional).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        OrImpl(Q1 &firstOperand, Q2 &secondOperand, Qs &...otherOperands)
            : ModifierClass(firstOperand, secondOperand, otherOperands...)
        {
        }

        ///
        /// @brief Get a std::optional filled with the data of the specified operand at index @p index if existing.
        ///
        /// @warning May throw exceptions, look at the specified operand type equivalent method documentation.
        ///
        /// @tparam operandId Id of the operand (where 0 is Q1, 1 is Q2...).
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return auto A std::optional filled with the resulting operand data at index @p index if existing.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        template <size_t operandId>
        inline typename ModifierClass::template OperandData<operandId> getOperandData(size_t index)
        {
            auto &operand = std::get<operandId>(this->_operands);

            if (index < getQueryableMask(operand).size() && getQueryableMask(operand)[index])
                return getQueryableData(operand, index);
            return std::nullopt;
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
            else {
                auto &leftOperand = std::get<0>(this->_operands);
                auto &rightOperand = std::get<1>(this->_operands);

                if (ecstasy::query::getQueryableMask(leftOperand).size()
                    > ecstasy::query::getQueryableMask(rightOperand).size()) {
                    this->_mask = ecstasy::query::getQueryableMask(leftOperand);
                    this->_mask.inplaceOr(ecstasy::query::getQueryableMask(rightOperand));
                } else {
                    this->_mask = ecstasy::query::getQueryableMask(rightOperand);
                    this->_mask.inplaceOr(ecstasy::query::getQueryableMask(leftOperand));
                }
            }
        }

      private:
        ///
        /// @brief Combine the current bitmask with the given one.
        ///
        /// @param[in] mask Evaluated bitmask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-21)
        ///
        inline void combineMask(const util::BitSet &mask)
        {
            if (this->_mask.size() < mask.size())
                this->_mask = util::BitSet(mask).inplaceOr(this->_mask);
            else
                this->_mask.inplaceOr(mask);
        }

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
            this->_mask = getQueryableMask(std::get<0>(this->_operands));
            combineMask(getQueryableMask(std::get<1>(this->_operands)));
            std::ignore = std::make_tuple((combineMask(getQueryableMask(std::get<ints + 2>(this->_operands))), 0)...);
        }
    };

    template <Queryable Q1, Queryable Q2, Queryable... Qs>
    using Or = OrImpl<false, Q1, Q2, Qs...>;
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIERS_OR_HPP_ */
