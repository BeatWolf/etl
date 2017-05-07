//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

#include "etl/stop.hpp"

namespace etl {

namespace detail {

template <typename E, typename Sequence>
struct build_fast_dyn_matrix_type;

template <typename E, size_t... I>
struct build_fast_dyn_matrix_type<E, std::index_sequence<I...>> {
    using type = fast_matrix_impl<
        value_t<E>,
        cpp::aligned_vector<value_t<E>, default_intrinsic_traits<value_t<E>>::alignment>,
        decay_traits<E>::storage_order,
        decay_traits<E>::template dim<I>()...>;
};

template <typename E, size_t... I>
decltype(auto) build_dyn_matrix_type(E&& expr, std::index_sequence<I...>){
    return dyn_matrix_impl<value_t<E>, decay_traits<E>::storage_order, decay_traits<E>::dimensions()>(etl::dim<I>(expr)...);
}

} // end of namespace detail

/*!
 * \brief Force a temporary out of the expression
 *
 * In case of a fast matrix, a fast matrix with vector storage is created  even
 * if the input has array storage.
 *
 * \param expr The expression to make a temporary from
 * \return a temporary of the expression
 */
template <typename E, cpp_enable_if(decay_traits<E>::is_fast)>
decltype(auto) force_temporary(E&& expr) {
    typename detail::build_fast_dyn_matrix_type<E, std::make_index_sequence<decay_traits<E>::dimensions()>>::type mat;
    mat = std::forward<E>(expr);
    return mat;
}

/*!
 * \brief Force a temporary out of the expression
 *
 * In case of a fast matrix, a fast matrix with vector storage is created  even
 * if the input has array storage.
 *
 * \param expr The expression to make a temporary from
 * \return a temporary of the expression
 */
template <typename E, cpp_enable_if(!decay_traits<E>::is_fast, !is_sparse_matrix<E>::value)>
decltype(auto) force_temporary(E&& expr) {
    dyn_matrix_impl<value_t<E>, decay_traits<E>::storage_order, decay_traits<E>::dimensions()> mat;
    mat = expr;
    return mat;
}

/*!
 * \brief Force a temporary out of the expression
 *
 * In case of a fast matrix, a fast matrix with vector storage is created  even
 * if the input has array storage.
 *
 * \param expr The expression to make a temporary from
 * \return a temporary of the expression
 */
template <typename E, cpp_enable_if(is_sparse_matrix<E>::value)>
decltype(auto) force_temporary(E&& expr) {
    //Sizes will be directly propagated
    return std::decay_t<E>{std::forward<E>(expr)};
}

/*!
 * \brief Force a dynamic temporary out of the expression
 *
 * This function will always return a dyn_matrix. This has the
 * advantage of the matrix being able to change dimensions
 * (transpose for instance). However, this cause fast matrix
 * dimensions to decay.
 *
 * \param expr The expression to make a temporary from
 * \return a temporary of the expression
 */
template <typename E>
decltype(auto) force_temporary_dyn(E&& expr) {
    //Sizes will be directly propagated
    dyn_matrix_impl<value_t<E>, decay_traits<E>::storage_order, decay_traits<E>::dimensions()> mat;
    mat = expr;
    return mat;
}

/*!
 * \brief Force a temporary out of the expression with the same
 * dimensions, but the content is not defined. The expression will
 * not be evaluated.
 *
 * \param expr The expression to make a temporary from
 * \return a temporary with the same dimensions as the expression
 */
template <typename E, cpp_enable_if(decay_traits<E>::is_fast)>
decltype(auto) force_temporary_dim_only(E&& expr) {
    cpp_unused(expr);
    return typename detail::build_fast_dyn_matrix_type<E, std::make_index_sequence<decay_traits<E>::dimensions()>>::type{};
}

/*!
 * \brief Force a temporary out of the expression with the same
 * dimensions, but the content is not defined. The expression will
 * not be evaluated.
 *
 * \param expr The expression to make a temporary from
 * \return a temporary with the same dimensions as the expression
 */
template <typename E, cpp_enable_if(!decay_traits<E>::is_fast)>
decltype(auto) force_temporary_dim_only(E&& expr) {
    return detail::build_dyn_matrix_type(expr, std::make_index_sequence<decay_traits<E>::dimensions()>());
}

/*!
 * \brief Make a temporary out of the expression if necessary
 *
 * A temporary is necessary when the expression has no direct access.
 *
 * \param expr The expression to make a temporary from
 * \return a temporary of the expression if necessary, otherwise the expression itself
 */
template <typename E, cpp_enable_if(has_direct_access<E>::value)>
decltype(auto) make_temporary(E&& expr) {
    return std::forward<E>(expr);
}

/*!
 * \brief Make a temporary out of the expression if necessary
 *
 * A temporary is necessary when the expression has no direct.
 *
 * \param expr The expression to make a temporary from
 * \return a temporary of the expression if necessary, otherwise the expression itself
 */
template <typename E, cpp_enable_if(!has_direct_access<E>::value)>
decltype(auto) make_temporary(E&& expr) {
    return force_temporary(std::forward<E>(expr));
}

} //end of namespace etl
