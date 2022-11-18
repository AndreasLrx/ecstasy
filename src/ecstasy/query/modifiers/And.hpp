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
    /// @brief Binary query modifier which performs a and between two queryables.
    ///
    /// @tparam Q1 Left operand queryable type.
    /// @tparam Q2 Right operand queryable type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-27)
    ///
    template <Queryable Q1, Queryable Q2>
    class And : public BinaryModifier {
      public:
        /// @brief Left operand type (queryable)
        using LeftOperand = Q1;

        /// @brief Left operand data type
        using LeftQueryData = LeftOperand::QueryData;

        /// @brief Right operand type (queryable)
        using RightOperand = Q2;

        /// @brief Right operand data type
        using RightQueryData = RightOperand::QueryData;

        /// @brief Wrapped queryables.
        using Operands = std::tuple<Q1, Q2>;

        /// @brief @ref Queryable constaint.
        using QueryData = std::tuple<LeftQueryData, RightQueryData>;

        ///
        /// @brief Construct a new And Queryable modifier.
        ///
        /// @param[in] leftOperand left queryable operand.
        /// @param[in] rightOperand right queryable operand.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        And(LeftOperand &leftOperand, RightOperand &rightOperand)
            : _leftOperand(leftOperand), _rightOperand(rightOperand)
        {
            reloadMask();
        }

        ///
        /// @brief Get the Mask of the internal queryable.
        /// The result is a binary And between the two operands bitset.
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
        /// @brief Get the operands data at the given index. The data are merged in a tuple containing the result of
        /// two operands data at the given index.
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
            return std::make_tuple(_leftOperand.getQueryData(index), _rightOperand.getQueryData(index));
        }

        ///
        /// @brief Update the mask to be up to date with the operands bitmasks.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        void reloadMask()
        {
            _mask = _leftOperand.getMask() & _rightOperand.getMask();
        }

      private:
        LeftOperand &_leftOperand;
        RightOperand &_rightOperand;
        util::BitSet _mask;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIERS_AND_HPP_ */
