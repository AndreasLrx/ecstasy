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
#include "util/meta/add_optional.hpp"

namespace ecstasy::query::modifier
{
    ///
    /// @brief Binary query modifier which performs a or between two queryables.
    ///
    /// @tparam Q1 Left operand queryable type.
    /// @tparam Q2 Right operand queryable type.
    /// @tparam Qs Additional operand queryable types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-27)
    ///
    template <Queryable Q1, Queryable Q2, Queryable... Qs>
    class Or : public BinaryModifier {
      public:
        /// @brief Left operand type (queryable)
        using LeftOperand = Q1;

        /// @brief Right operand type (queryable)
        using RightOperand = Q2;

        /// @brief Operand types (queryable)
        using Operands = std::tuple<Q1 &, Q2 &, Qs &...>;

        /// @brief @ref Queryable constaint.
        // clang-format off
        using QueryData = std::tuple<
            util::meta::add_optional_t<typename Q1::QueryData>,
            util::meta::add_optional_t<typename Q2::QueryData>,
            util::meta::add_optional_t<typename Qs::QueryData>...>;
        // clang-format on

        ///
        /// @brief Construct a new Or Queryable modifier.
        ///
        /// @param[in] leftOperand left queryable operand.
        /// @param[in] rightOperand right queryable operand.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        Or(Q1 &firstOperand, Q2 &secondOperand, Qs &...otherOperands)
            : _operands({firstOperand, secondOperand, otherOperands...})
        {
            reloadMask();
        }

        ///
        /// @brief Get the Mask of the internal queryable.
        /// The result is a binary Or between the two operands bitset.
        ///
        /// @note @ref Queryable constraint.
        /// @warning Use reload masks if the operand masks have changed since the construction.
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
        /// @brief Get a std::optional filled with the data of the left operand at index @p index if existing.
        ///
        /// @warning May throw exceptions, look at the @b LeftOperand type equivalent method documentation.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return @ref LeftQueryData A std::optional filled with the left operand data at index @p index if existing.
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
        /// @brief Get the operands data at the given index. The data are merged in a tuple containing the result of
        /// @ref getLeftQueryData() and @ref getRightQueryData().
        ///
        /// @note @ref Queryable constraint.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return @ref QueryData A tuple of std::optional containing the operands data if existing.
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
            else {
                auto &leftOperand = std::get<0>(_operands);
                auto &rightOperand = std::get<1>(_operands);

                if (leftOperand.getMask().size() > rightOperand.getMask().size()) {
                    _mask = leftOperand.getMask();
                    _mask.inplaceOr(rightOperand.getMask());
                } else {
                    _mask = rightOperand.getMask();
                    _mask.inplaceOr(leftOperand.getMask());
                }
            }
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
        /// @brief Combine the current bitmask with the given one.
        ///
        /// @param[in] mask Evaluated bitmask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-21)
        ///
        void combineMask(const util::BitSet &mask)
        {
            if (_mask.size() < mask.size())
                _mask = util::BitSet(mask).inplaceAnd(_mask);
            else
                _mask.inplaceOr(mask);
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
            _mask = std::get<0>(_operands).getMask();
            combineMask(getRight().getMask());
            combineMask(std::get<ints + 2>(_operands).getMask()...);
        }

        Operands _operands;
        util::BitSet _mask;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIERS_OR_HPP_ */
