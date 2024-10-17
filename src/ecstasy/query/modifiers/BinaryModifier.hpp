///
/// @file And.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-27
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_MODIFIERS_BINARYMODIFIER_HPP_
#define ECSTASY_QUERY_MODIFIERS_BINARYMODIFIER_HPP_

#include "Modifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"

namespace ecstasy::query::modifier
{
    ///
    /// @brief Base class of query modifier operating on at least two queryables.
    ///
    /// @tparam Derived Derived class implementing the modifier.
    /// @tparam AutoLock Lock the @ref Lockable queryables if true.
    /// @tparam DataModifier Meta class implementing a type modifying the queryable data types (example std::optional).
    /// @tparam Q1 Left operand queryable type.
    /// @tparam Q2 Right operand queryable type.
    /// @tparam Qs Additional operand queryable types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-27)
    ///
    template <typename Derived, bool AutoLock, template <typename> class DataModifier, Queryable Q1, Queryable Q2,
        Queryable... Qs>
    class BinaryModifier : public Modifier<AutoLock> {
      public:
        /// @brief @ref Modifier constraint.
        using Operands = util::meta::Traits<Q1, Q2, Qs...>;
        using Internal = std::tuple<queryable_qualifiers_t<Q1, AutoLock>, queryable_qualifiers_t<Q2, AutoLock>,
            queryable_qualifiers_t<Qs, AutoLock>...>;

        /// @brief @ref ecstasy::query::Queryable constaint.
        // clang-format off
        using QueryData = std::tuple<
            typename DataModifier<typename Q1::QueryData>::type,
            typename DataModifier<typename Q2::QueryData>::type,
            typename DataModifier<typename Qs::QueryData>::type...>;
        // clang-format on

        /// @brief QueryData for the given operand.
        template <size_t operandId>
        using OperandData = std::tuple_element_t<operandId, QueryData>;

        ///
        /// @brief Construct a new binary query modifier.
        ///
        /// @param[in] firstOperand left queryable operand.
        /// @param[in] secondOperand right queryable operand.
        /// @param[in] otherOperands additional operands (optional).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        BinaryModifier(Q1 &firstOperand, Q2 &secondOperand, Qs &...otherOperands)
            : _operands(firstOperand, secondOperand, otherOperands...)
        {
            static_cast<Derived &>(*this).reloadMask();
        }

        ///
        /// @brief Get the Mask of the internal queryables.
        ///
        /// @note @ref ecstasy::query::Queryable constraint.
        /// @warning Use @p reloadMask() if the operand masks have changed since the construction.
        ///
        /// @return const util::BitSet& resulting mask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        [[nodiscard]] constexpr const util::BitSet &getMask() const noexcept
        {
            return _mask;
        }

        ///
        /// @brief Get the operands data at the given index.
        ///
        /// @note @ref ecstasy::query::Queryable constraint.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return @ref QueryData A tuple containing the operands data.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-27)
        ///
        [[nodiscard]] QueryData getQueryData(size_t index)
        {
            return getQueryData(index, std::make_index_sequence<(sizeof...(Qs))>());
        }

      protected:
        /// @internal
        /// @brief Get the Derived Operand Data
        ///
        /// @tparam operandId Operand id.
        /// @param[in] index Index of the entity.
        ///
        /// @return OperandData<operandId> The operand data at index @p index.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        template <size_t operandId>
        inline OperandData<operandId> getDerivedOperandData(size_t index)
        {
            return static_cast<Derived &>(*this).template getOperandData<operandId>(index);
        }

        ///
        /// @brief Get the query data.
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
        inline QueryData getQueryData(size_t index, [[maybe_unused]] std::integer_sequence<size_t, ints...> int_seq)
        {
            return {getDerivedOperandData<0>(index), getDerivedOperandData<1>(index),
                getDerivedOperandData<ints + 2>(index)...};
        }

        /// @brief Tuple containing the queryables.
        Internal _operands;
        /// @brief Mask of the queryables.
        util::BitSet _mask;
    };
} // namespace ecstasy::query::modifier

#endif /* !ECSTASY_QUERY_MODIFIERS_BINARYMODIFIER_HPP_ */
