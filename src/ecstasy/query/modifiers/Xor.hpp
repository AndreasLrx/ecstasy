///
/// @file Xor.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-27
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_MODIFIERS_XOR_HPP_
#define ECSTASY_QUERY_MODIFIERS_XOR_HPP_

#include "Modifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "ecstasy/query/concepts/add_optional.hpp"

namespace ecstasy::query::modifier
{
    ///
    /// @brief Binary query modifier which performs a xor between two queryables.
    ///
    /// @tparam Q1 Left operand queryable type.
    /// @tparam Q2 Right operand queryable type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-27)
    ///
    template <Queryable Q1, Queryable Q2>
    class Xor : public BinaryModifier {
      public:
        using LeftOperand = Q1;
        using LeftQueryData = add_optional_t<typename LeftOperand::QueryData>;
        using RightOperand = Q2;
        using RightQueryData = add_optional_t<typename RightOperand::QueryData>;

        /// @brief Wrapped queryables.
        using Operands = std::tuple<Q1, Q2>;
        /// @brief @ref Queryable constaint.
        using QueryData = std::tuple<LeftQueryData, RightQueryData>;

        ///
        /// @brief Construct a new Xor Queryable modifier.
        ///
        /// @param[in] leftOperand left queryable operand.
        /// @param[in] rightOperand right queryable operand.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        Xor(LeftOperand &leftOperand, RightOperand &rightOperand)
            : _leftOperand(leftOperand), _rightOperand(rightOperand)
        {
            reloadMask();
        }

        ///
        /// @brief Get the Mask of the internal queryable.
        /// The result is a binary Xor between the two operands bitset.
        ///
        /// @note @ref Queryable constraint.
        /// @warning Use reload masks if the operand masks have changed since the construction.
        ///
        /// @return constexpr const util::BitSet& resulting mask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        constexpr const util::BitSet &getMask() const
        {
            return _mask;
        }

        ///
        /// @brief Get a std::optional filled with the data of the left operand at index @p index if existing.
        ///
        /// @warning May throw exceptions, look at the @b LeftOperand type equivalent function documentation.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return LeftQueryData A std::optional filled with the left operand data at index @p index if existing.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        LeftQueryData getLeftQueryData(size_t index)
        {
            if (index < _leftOperand.getMask().size() && _leftOperand.getMask()[index])
                return LeftQueryData{_leftOperand.getQueryData(index)};
            return std::nullopt;
        }

        ///
        /// @brief Get a std::optional filled with the data of the right operand at index @p index if existing.
        ///
        /// @warning May throw exceptions, look at the @b RightOperand type equivalent function documentation.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return RightQueryData A std::optional filled with the right operand data at index @p index if existing.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        RightQueryData getRightQueryData(size_t index)
        {
            if (index < _rightOperand.getMask().size() && _rightOperand.getMask()[index])
                return RightQueryData{_rightOperand.getQueryData(index)};
            return std::nullopt;
        }

        ///
        /// @brief Get the operands data at the given index. The data are merged in a tuple containing the result of
        /// @ref getLeftQueryData() and @ref getRightQueryData().
        ///
        /// @note @ref Queryable constraint.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return QueryData A tuple of std::optional containing the operands data if existing.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        QueryData getQueryData(size_t index)
        {
            return std::make_tuple(getLeftQueryData(index), getRightQueryData(index));
        }

        ///
        /// @brief Update the mask to be up to date with the operands bitmasks.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        void reloadMask()
        {
            if (_leftOperand.getMask().size() > _rightOperand.getMask().size()) {
                _mask = _leftOperand.getMask();
                _mask.inplaceXor(_rightOperand.getMask());
            } else {
                _mask = _rightOperand.getMask();
                _mask.inplaceXor(_leftOperand.getMask());
            }
        }

      private:
        LeftOperand &_leftOperand;
        RightOperand &_rightOperand;
        util::BitSet _mask;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIERS_XOR_HPP_ */
