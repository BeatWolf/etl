//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains all the operators and functions to build expressions
 * representing mathematical functions.
*/

#pragma once

namespace etl {

/*!
 * \brief Apply square root on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the square root of each value of the given expression
 */
template <typename E>
auto sqrt(E&& value) -> detail::unary_helper<E, sqrt_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::sqrt can only be used on ETL expressions");
    return detail::unary_helper<E, sqrt_unary_op>{value};
}

/*!
 * \brief Apply inverse square root on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the inverse square root of each value of the given expression
 */
template <typename E>
auto invsqrt(E&& value) -> detail::unary_helper<E, invsqrt_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::invsqrt can only be used on ETL expressions");
    return detail::unary_helper<E, invsqrt_unary_op>{value};
}

/*!
 * \brief Apply cubic root on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the cubic root of each value of the given expression
 */
template <typename E>
auto cbrt(E&& value) -> detail::unary_helper<E, cbrt_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::cbrt can only be used on ETL expressions");
    return detail::unary_helper<E, cbrt_unary_op>{value};
}

/*!
 * \brief Apply inverse cubic root on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the inverse cubic root of each value of the given expression
 */
template <typename E>
auto invcbrt(E&& value) -> detail::unary_helper<E, invcbrt_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::invcbrt can only be used on ETL expressions");
    return detail::unary_helper<E, invcbrt_unary_op>{value};
}

/*!
 * \brief Apply logarithm on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the logarithm of each value of the given expression
 */
template <typename E>
auto log(E&& value) -> detail::unary_helper<E, log_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::log can only be used on ETL expressions");
    return detail::unary_helper<E, log_unary_op>{value};
}

/*!
 * \brief Apply tangent on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the tangent of each value of the given expression
 */
template <typename E>
auto tan(E&& value) -> detail::unary_helper<E, tan_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::tan can only be used on ETL expressions");
    return detail::unary_helper<E, tan_unary_op>{value};
}

/*!
 * \brief Apply cosinus on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the cosinus of each value of the given expression
 */
template <typename E>
auto cos(E&& value) -> detail::unary_helper<E, cos_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::cos can only be used on ETL expressions");
    return detail::unary_helper<E, cos_unary_op>{value};
}

/*!
 * \brief Apply sinus on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the sinus of each value of the given expression
 */
template <typename E>
auto sin(E&& value) -> detail::unary_helper<E, sin_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::sin can only be used on ETL expressions");
    return detail::unary_helper<E, sin_unary_op>{value};
}

/*!
 * \brief Apply hyperbolic tangent on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the hyperbolic tangent of each value of the given expression
 */
template <typename E>
auto tanh(E&& value) -> detail::unary_helper<E, tanh_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::tanh can only be used on ETL expressions");
    return detail::unary_helper<E, tanh_unary_op>{value};
}

/*!
 * \brief Apply hyperbolic cosinus on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the hyperbolic cosinus of each value of the given expression
 */
template <typename E>
auto cosh(E&& value) -> detail::unary_helper<E, cosh_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::cosh can only be used on ETL expressions");
    return detail::unary_helper<E, cosh_unary_op>{value};
}

/*!
 * \brief Apply hyperbolic sinus on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the hyperbolic sinus of each value of the given expression
 */
template <typename E>
auto sinh(E&& value) -> detail::unary_helper<E, sinh_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::sinh can only be used on ETL expressions");
    return detail::unary_helper<E, sinh_unary_op>{value};
}

/*!
 * \brief Apply exponential on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the exponential of each value of the given expression
 */
template <typename E>
auto exp(E&& value) -> detail::unary_helper<E, exp_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::exp can only be used on ETL expressions");
    return detail::unary_helper<E, exp_unary_op>{value};
}

/*!
 * \brief Apply sign on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the sign of each value of the given expression
 */
template <typename E>
auto sign(E&& value) -> detail::unary_helper<E, sign_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::sign can only be used on ETL expressions");
    return detail::unary_helper<E, sign_unary_op>{value};
}

/*!
 * \brief Performs the identiy function on the ETL expression.
 * \param value The ETL expression
 * \return The same value, perfectly forwardd
 */
template <typename E>
decltype(auto) identity(E&& value) {
    return std::forward<E>(value);
}

/*!
 * \brief Return the derivative of the identiy function for the given value.
 * \param value The ETL expression
 * \return 1.0
 */
template <typename E>
auto identity_derivative(E&& value) {
    cpp_unused(value);
    return 1.0;
}

//Note: Use of decltype here should not be necessary, but g++ does
//not like it without it for some reason

#ifdef ETL_CUDNN_MODE

/*!
 * \brief Return the logistic sigmoid of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the logistic sigmoid of the input.
 */
template <typename E, cpp_enable_if(all_dma<E>::value)>
auto sigmoid(E&& value) -> unary_function_expr<detail::build_type<E>, detail::sigmoid> {
    static_assert(is_etl_expr<E>::value, "etl::sigmoid can only be used on ETL expressions");
    return unary_function_expr<detail::build_type<E>, detail::sigmoid>(value);
}

/*!
 * \brief Return the logistic sigmoid of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the logistic sigmoid of the input.
 */
template <typename E, cpp_disable_if(all_dma<E>::value)>
auto sigmoid(E&& value) -> decltype(1.0 / (1.0 + exp(-value))) {
    static_assert(is_etl_expr<E>::value, "etl::sigmoid can only be used on ETL expressions");
    return 1.0 / (1.0 + exp(-value));
}

/*!
 * \brief Return the relu activation of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the relu activation of the input.
 */
template <typename E, cpp_enable_if(all_dma<E>::value)>
auto relu(E&& value) -> unary_function_expr<detail::build_type<E>, detail::relu> {
    static_assert(is_etl_expr<E>::value, "etl::relu can only be used on ETL expressions");
    return unary_function_expr<detail::build_type<E>, detail::relu>(value);
}

/*!
 * \brief Return the relu activation of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the relu activation of the input.
 */
template <typename E, cpp_disable_if(all_dma<E>::value)>
auto relu(E&& value) -> decltype(max(value, 0.0)) {
    static_assert(is_etl_expr<E>::value, "etl::relu can only be used on ETL expressions");
    return max(value, 0.0);
}

#else

/*!
 * \brief Return the logistic sigmoid of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the logistic sigmoid of the input.
 */
template <typename E>
auto sigmoid(E&& value) -> decltype(1.0 / (1.0 + exp(-value))) {
    static_assert(is_etl_expr<E>::value, "etl::sigmoid can only be used on ETL expressions");
    return 1.0 / (1.0 + exp(-value));
}

/*!
 * \brief Return the relu activation of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the relu activation of the input.
 */
template <typename E>
auto relu(E&& value) -> decltype(max(value, 0.0)) {
    static_assert(is_etl_expr<E>::value, "etl::relu can only be used on ETL expressions");
    return max(value, 0.0);
}

#endif

/*!
 * \brief Return the derivative of the logistic sigmoid of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the derivative of the logistic sigmoid of the input.
 */
template <typename E>
auto sigmoid_derivative(E&& value) -> decltype(sigmoid(value) >> (1.0 - sigmoid(value))) {
    static_assert(is_etl_expr<E>::value, "etl::sigmoid_derivative can only be used on ETL expressions");
    return sigmoid(value) >> (1.0 - sigmoid(value));
}

/*!
 * \brief Return a fast approximation of the logistic sigmoid of the given ETL expression.
 *
 * This function is faster than the sigmoid function and has an acceptable precision.
 *
 * \param value The ETL expression
 * \return An ETL expression representing a fast approximation of the logistic sigmoid of the input.
 */
template <typename E>
auto fast_sigmoid(const E& value) -> detail::unary_helper<E, fast_sigmoid_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::fast_sigmoid can only be used on ETL expressions");
    return detail::unary_helper<E, fast_sigmoid_unary_op>{value};
}

/*!
 * \brief Return an hard approximation of the logistic sigmoid of the given ETL expression.
 *
 * This function is much faster than the sigmoid, but it's precision is very low.
 *
 * \param x The ETL expression
 * \return An ETL expression representing an hard approximation of the logistic sigmoid of the input.
 */
template <typename E>
auto hard_sigmoid(E&& x) -> decltype(etl::clip(x * 0.2 + 0.5, 0.0, 1.0)) {
    static_assert(is_etl_expr<E>::value, "etl::hard_sigmoid can only be used on ETL expressions");
    return etl::clip(x * 0.2 + 0.5, 0.0, 1.0);
}

/*!
 * \brief Return the softmax function of the given ETL expression.
 * \param e The ETL expression
 * \return An ETL expression representing the softmax function of the input.
 */
template <typename E>
auto softmax(E&& e) {
    static_assert(is_etl_expr<E>::value, "etl::softmax can only be used on ETL expressions");
    return exp(e) / sum(exp(e));
}

/*!
 * \brief Returns the softmax function of the given ETL expression.
 * This version is implemented so that numerical stability is preserved.
 * \param e The ETL expression
 * \return An ETL expression representing the softmax function of the input.
 */
template <typename E>
auto stable_softmax(E&& e) {
    static_assert(is_etl_expr<E>::value, "etl::stable_softmax can only be used on ETL expressions");
    auto m = max(e);
    return exp(e - m) / sum(exp(e - m));
}

/*!
 * \brief Return the derivative of the softmax function of the given ETL expression.
 * \param e The ETL expression
 * \return An ETL expression representing the derivative of the softmax function of the input.
 */
template <typename E>
auto softmax_derivative(E&& e) {
    cpp_unused(e);
    return 1.0;
}

/*!
 * \brief Return the softplus of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the softplus of the input.
 */
template <typename E>
auto softplus(E&& value) -> decltype(log(1.0 + exp(value))) {
    static_assert(is_etl_expr<E>::value, "etl::softplus can only be used on ETL expressions");
    return log(1.0 + exp(value));
}

/*!
 * \brief Apply Bernoulli sampling to the values of the expression
 * \param value the expression to sample
 * \return an expression representing the Bernoulli sampling of the given expression
 */
template <typename E>
auto bernoulli(const E& value) -> detail::unary_helper<E, bernoulli_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::bernoulli can only be used on ETL expressions");
    return detail::unary_helper<E, bernoulli_unary_op>{value};
}

/*!
 * \brief Apply Reverse Bernoulli sampling to the values of the expression
 * \param value the expression to sample
 * \return an expression representing the Reverse Bernoulli sampling of the given expression
 */
template <typename E>
auto r_bernoulli(const E& value) -> detail::unary_helper<E, reverse_bernoulli_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::r_bernoulli can only be used on ETL expressions");
    return detail::unary_helper<E, reverse_bernoulli_unary_op>{value};
}

/*!
 * \brief Return the derivative of the tanh function of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the derivative of the tanh function of the input.
 */
template <typename E>
auto tanh_derivative(E&& value) -> decltype(1.0 - (tanh(value) >> tanh(value))) {
    static_assert(is_etl_expr<E>::value, "etl::tanh_derivative can only be used on ETL expressions");
    return 1.0 - (tanh(value) >> tanh(value));
}

/*!
 * \brief Return the derivative of the relu function of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the derivative of the relu function of the input.
 */
template <typename E>
auto relu_derivative(const E& value) -> detail::unary_helper<E, relu_derivative_op> {
    static_assert(is_etl_expr<E>::value, "etl::relu_derivative can only be used on ETL expressions");
    return detail::unary_helper<E, relu_derivative_op>{value};
}

} //end of namespace etl
