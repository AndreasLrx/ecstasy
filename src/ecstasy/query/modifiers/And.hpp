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

#include "Modifier.hpp"
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
    class And : public Modifier {
      public:
        /// @brief @ref Modifier constraint.
        using Operands = std::tuple<Q1, Q2, Qs...>;

        /// @brief @ref Queryable constaint.
        // clang-format off
        using QueryData = std::tuple<
            typename Q1::QueryData,
            typename Q2::QueryData,
            typename Qs::QueryData...>;
        // clang-format on

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
            : _operands({firstOperand, secondOperand, otherOperands...})
        {
            reloadMask();
        }

        ///
        /// @brief Get the Mask of the internal queryables.
        /// The result is a binary And between all the operands bitset's.
        ///
        /// @note @ref Queryable constraint.
        /// @warning Use @ref reloadMask() if the operand masks have changed since the construction.
        ///
        /// @return const util::BitSet& resulting mask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        constexpr const util::BitSet &getMask() const
        {
            return _mask;
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
        auto getOperandData(size_t operandId, size_t index)
        {
            auto &operand = std::get<operandId>(_operands);

            if (index < operand.getMask().size() && operand.getMask()[index])
                return operand.getQueryData(index);
            return std::nullopt;
        }

        ///
        /// @brief Get the operands data at the given index.
        ///
        /// @note @ref Queryable constraint.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return @ref QueryData A tuple containing the operands data.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        QueryData getQueryData(size_t index)
        {
            return getQueryData(index, std::make_index_sequence<(sizeof...(Qs))>());
        }

        ///
        /// @brief Update the mask to be up to date with the operands bitmasks.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        void reloadMask()
        {
            if constexpr (sizeof...(Qs) > 0)
                combineOperandMasks(std::make_index_sequence<(sizeof...(Qs))>());
            else
                _mask = std::get<0>(_operands).getMask() & std::get<1>(_operands).getMask();
        }

      private:
        ///
        /// @brief Get the the query data.
        ///
        /// @tparam ints Sequence values.
        ///
        /// @param[in] index Id of the query data to fetch.
        /// @param[in] int_seq Sequence containing the queryables ids.
        ///
        /// @return @ref QueryData A tuple of std::optional containing the operands data if existing.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-21)
        ///
        template <size_t... ints>
        QueryData getQueryData(size_t index, std::integer_sequence<size_t, ints...> int_seq)
        {
            (void)int_seq;
            return std::make_tuple(
                getOperandData(0, index), getOperandData(1, index), getOperandData(ints + 2, index)...);
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
        void combineOperandMasks(std::integer_sequence<size_t, ints...> int_seq)
        {
            (void)int_seq;
            _mask = ((util::BitSet(std::get<0>(_operands).getMask()) & std::get<1>(_operands).getMask()) &= ... &=
                std::get<ints + 2>(_operands).getMask());
        }

        std::tuple<Q1 &, Q2 &, Qs &...> _operands;
        util::BitSet _mask;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIERS_AND_HPP_ */
