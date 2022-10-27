///
/// @file Or.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-27
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_MODIFIERS_OR_HPP_
#define ECSTASY_QUERY_MODIFIERS_OR_HPP_

#include "Modifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "ecstasy/query/concepts/add_optional.hpp"

namespace ecstasy::query::modifier
{
    ///
    /// @brief Binary query modifier which performs a or between two queryables.
    ///
    /// @tparam Internal Type of the wrapped queryable.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <Queryable Q1, Queryable Q2>
    class Or : public BinaryModifier {
      public:
        /// @brief Wrapped queryables.
        using LeftOperand = Q1;
        using RightOperand = Q2;
        using Operands = std::tuple<Q1, Q2>;
        /// @brief @ref Queryable constaint.
        using LeftQueryData = add_optional_t<typename LeftOperand::QueryData>;
        using RightQueryData = add_optional_t<typename RightOperand::QueryData>;
        using QueryData = std::tuple<LeftQueryData, RightQueryData>;

        Or(LeftOperand &leftOperand, RightOperand &rightOperand)
            : _leftOperand(leftOperand), _rightOperand(rightOperand)
        {
            reloadMask();
        }

        constexpr const util::BitSet &getMask() const
        {
            return _mask;
        }

        LeftQueryData getLeftQueryData(size_t index)
        {
            if (index < _leftOperand.getMask().size() && _leftOperand.getMask()[index])
                return LeftQueryData{_leftOperand.getQueryData(index)};
            return std::nullopt;
        }

        RightQueryData getRightQueryData(size_t index)
        {
            if (index < _rightOperand.getMask().size() && _rightOperand.getMask()[index])
                return RightQueryData{_rightOperand.getQueryData(index)};
            return std::nullopt;
        }

        QueryData getQueryData(size_t index)
        {
            return std::make_tuple(getLeftQueryData(index), getRightQueryData(index));
        }

        void reloadMask()
        {
            if (_leftOperand.getMask().size() > _rightOperand.getMask().size()) {
                _mask = _leftOperand.getMask();
                _mask.inplaceOr(_rightOperand.getMask());
            } else {
                _mask = _rightOperand.getMask();
                _mask.inplaceOr(_leftOperand.getMask());
            }
        }

      private:
        LeftOperand &_leftOperand;
        RightOperand &_rightOperand;
        util::BitSet _mask;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIERS_OR_HPP_ */
