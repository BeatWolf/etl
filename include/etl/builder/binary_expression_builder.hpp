//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains all the operators and functions to build binary expressions.
*/

#pragma once

//Include implementations
#include "etl/impl/scalar_op.hpp"

namespace etl {

/*!
 * \brief Builds an expression representing the subtraction of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the subtraction of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto operator-(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, minus_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the addition of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the addition of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto operator+(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, plus_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the scalar multipliation of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the scalar multipliation of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto operator>>(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, mul_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the scalar multiplication of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the scalar multiplication of lhs and rhs
 */
template <typename LE, typename RE>
auto scale(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, mul_binary_op> {
    validate_expression(lhs, rhs);

    return detail::left_binary_helper<LE, RE, mul_binary_op>{lhs, rhs};
}

/*!
 * \brief Builds an expression representing the division of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the division of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto operator/(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, div_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the modulo of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the modulo of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto operator%(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, mod_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

// Mix scalars and ETL expressions (vector,matrix,binary,unary)

/*!
 * \brief Builds an expression representing the subtraction of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the subtraction of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto operator-(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, minus_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the subtraction of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the subtraction of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto operator-(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, minus_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the addition of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the addition of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto operator+(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, plus_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the addition of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the addition of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto operator+(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, plus_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the multiplication of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the multiplication of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto operator*(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, mul_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the multiplication of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the multiplication of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto operator*(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, mul_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the multiplication of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the multiplication of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto operator>>(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, mul_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the multiplication of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the multiplication of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto operator>>(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, mul_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the division of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the division of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE> && (is_div_strict || !std::is_floating_point<RE>::value))>
auto operator/(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, div_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the division of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the division of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE> && !is_div_strict, std::is_floating_point<RE>::value)>
auto operator/(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, mul_binary_op> {
    return {lhs, scalar<value_t<LE>>(value_t<LE>(1.0) / rhs)};
}

/*!
 * \brief Builds an expression representing the division of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the division of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto operator/(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, div_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the modulo of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the modulo of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto operator%(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, mod_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the modulo of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the modulo of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto operator%(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, mod_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

// Compound operators

/*!
 * \brief Compound addition of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value && is_simple_lhs<LE>)>
LE& operator+=(LE&& lhs, RE rhs) {
    detail::scalar_add::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound addition of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE> && is_simple_lhs<LE>)>
LE& operator+=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_add_to(lhs);
    return lhs;
}

/*!
 * \brief Compound subtraction of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value && is_simple_lhs<LE>)>
LE& operator-=(LE&& lhs, RE rhs) {
    detail::scalar_sub::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound subtraction of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE> && is_simple_lhs<LE>)>
LE& operator-=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_sub_to(lhs);
    return lhs;
}

/*!
 * \brief Compound multiplication of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value && is_simple_lhs<LE>)>
LE& operator*=(LE&& lhs, RE rhs) {
    detail::scalar_mul::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound multiplication of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE> && is_simple_lhs<LE>)>
LE& operator*=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_mul_to(lhs);
    return lhs;
}

/*!
 * \brief Compound multiplication of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value && is_simple_lhs<LE>)>
LE& operator>>=(LE&& lhs, RE rhs) {
    detail::scalar_mul::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound multiplication of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE> && is_simple_lhs<LE>)>
LE& operator>>=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_mul_to(lhs);
    return lhs;
}

/*!
 * \brief Compound division of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value && is_simple_lhs<LE>)>
LE& operator/=(LE&& lhs, RE rhs) {
    detail::scalar_div::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound division of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE> && is_simple_lhs<LE>)>
LE& operator/=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_div_to(lhs);
    return lhs;
}

/*!
 * \brief Compound modulo of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value && is_simple_lhs<LE>)>
LE& operator%=(LE&& lhs, RE rhs) {
    detail::scalar_mod::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound modulo of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE> && is_simple_lhs<LE>)>
LE& operator%=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_mod_to(lhs);
    return lhs;
}

// Comparison

/*!
 * \brief Builds an expression representing the elementwise comparison of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise comparison of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto equal(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, LE, equal_binary_op> {
    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the elementwise comparison of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise comparison of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto equal(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, scalar<value_t<LE>>, equal_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the elementwise comparison of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise comparison of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto equal(LE lhs, RE&& rhs) -> detail::bool_right_binary_helper<scalar<value_t<RE>>, RE, equal_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the elementwise comparison of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise comparison of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto not_equal(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, LE, not_equal_binary_op> {
    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the elementwise comparison of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise comparison of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto not_equal(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, scalar<value_t<LE>>, not_equal_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the elementwise comparison of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise comparison of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto not_equal(LE lhs, RE&& rhs) -> detail::bool_right_binary_helper<scalar<value_t<RE>>, RE, not_equal_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the elementwise less than comparison of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise less than comparison of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto less(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, LE, less_binary_op> {
    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the elementwise less than comparison of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise less than comparison of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto less(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, scalar<value_t<LE>>, less_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the elementwise less than comparison of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise less than comparison of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto less(LE lhs, RE&& rhs) -> detail::bool_right_binary_helper<scalar<value_t<RE>>, RE, less_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the elementwise less than or equals comparison of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise less than or equals comparison of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto less_equal(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, LE, less_equal_binary_op> {
    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the elementwise less than or equals comparison of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise less than or equals comparison of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto less_equal(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, scalar<value_t<LE>>, less_equal_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the elementwise less than or equals comparison of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise less than or equals comparison of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto less_equal(LE lhs, RE&& rhs) -> detail::bool_right_binary_helper<scalar<value_t<RE>>, RE, less_equal_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the elementwise greater than comparison of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise greater than comparison of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto greater(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, LE, greater_binary_op> {
    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the elementwise greater than comparison of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise greater than comparison of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto greater(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, scalar<value_t<LE>>, greater_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the elementwise greater than comparison of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise greater than comparison of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto greater(LE lhs, RE&& rhs) -> detail::bool_right_binary_helper<scalar<value_t<RE>>, RE, greater_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the elementwise greater than or equals comparison of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise greater than or equals comparison of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto greater_equal(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, LE, greater_equal_binary_op> {
    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the elementwise greater than or equals comparison of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise greater than or equals comparison of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto greater_equal(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, scalar<value_t<LE>>, greater_equal_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the elementwise greater than or equals comparison of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise greater than or equals comparison of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto greater_equal(LE lhs, RE&& rhs) -> detail::bool_right_binary_helper<scalar<value_t<RE>>, RE, greater_equal_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

// Logical operators

/*!
 * \brief Builds an expression representing the elementwise logical and of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise logical and of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto logical_and(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, LE, logical_and_binary_op> {
    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the elementwise logical and of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise logical and of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto logical_and(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, scalar<value_t<LE>>, logical_and_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the elementwise logical and of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise logical and of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto logical_and(LE lhs, RE&& rhs) -> detail::bool_right_binary_helper<scalar<value_t<RE>>, RE, logical_and_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the elementwise logical xor of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise logical xor of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto logical_xor(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, LE, logical_xor_binary_op> {
    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the elementwise logical xor of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise logical xor of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto logical_xor(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, scalar<value_t<LE>>, logical_xor_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the elementwise logical xor of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise logical xor of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto logical_xor(LE lhs, RE&& rhs) -> detail::bool_right_binary_helper<scalar<value_t<RE>>, RE, logical_xor_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the elementwise logical or of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise logical or of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(all_etl_expr<LE, RE>)>
auto logical_or(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, LE, logical_or_binary_op> {
    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the elementwise logical or of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise logical or of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value && is_etl_expr<LE>)>
auto logical_or(LE&& lhs, RE rhs) -> detail::bool_left_binary_helper<LE, scalar<value_t<LE>>, logical_or_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the elementwise logical or of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the element wise logical or of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value && is_etl_expr<RE>)>
auto logical_or(LE lhs, RE&& rhs) -> detail::bool_right_binary_helper<scalar<value_t<RE>>, RE, logical_or_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

} //end of namespace etl
