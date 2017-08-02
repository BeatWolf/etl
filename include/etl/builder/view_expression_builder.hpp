//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains all the operators and functions to build expressions for
 * views.
*/

#pragma once

namespace etl {

/*!
 * \brief Return a view representing the ith Dth dimension.
 * \param i The index to consider in the view
 * \tparam D The dimension to consider
 * \return a view representing the ith Dth dimension.
 */
template <size_t D, typename E>
auto dim(E&& value, size_t i) -> detail::identity_helper<E, dim_view<detail::build_identity_type<E>, D>> {
    static_assert(is_etl_expr<E>::value, "etl::dim can only be used on ETL expressions");
    return detail::identity_helper<E, dim_view<detail::build_identity_type<E>, D>>{{value, i}};
}

/*!
 * \brief Returns view representing the ith row of the given expression
 * \param value The ETL expression
 * \param i The row index
 * \return a view expression representing the ith row of the given expression
 */
template <typename E>
auto row(E&& value, size_t i) -> detail::identity_helper<E, dim_view<detail::build_identity_type<E>, 1>> {
    static_assert(is_etl_expr<E>::value, "etl::row can only be used on ETL expressions");
    return detail::identity_helper<E, dim_view<detail::build_identity_type<E>, 1>>{{value, i}};
}

/*!
 * \brief Returns view representing the ith column of the given expression
 * \param value The ETL expression
 * \param i The column index
 * \return a view expression representing the ith column of the given expression
 */
template <typename E>
auto col(E&& value, size_t i) -> detail::identity_helper<E, dim_view<detail::build_identity_type<E>, 2>> {
    static_assert(is_etl_expr<E>::value, "etl::col can only be used on ETL expressions");
    return detail::identity_helper<E, dim_view<detail::build_identity_type<E>, 2>>{{value, i}};
}

/*!
 * \brief Returns view representing a sub dimensional view of the given expression.
 * \param value The ETL expression
 * \param i The first index
 * \return a view expression representing a sub dimensional view of the given expression
 */
template <typename E>
auto sub(E&& value, size_t i) -> sub_view<detail::build_identity_type<E>, false> {
    static_assert(is_etl_expr<E>::value, "etl::sub can only be used on ETL expressions");
    static_assert(etl_traits<std::decay_t<E>>::dimensions() > 1, "Cannot use sub on vector");
    return {value, i};
}

/*!
 * \brief Returns view representing a sub matrix view of the given expression.
 * \param value The ETL expression
 * \param i The first index
 * \param j The second index
 * \param m The first dimension
 * \param n The second dimension
 * \return a view expression representing a sub matrix view of the given expression
 */
template <typename E>
auto sub(E&& value, size_t i, size_t j, size_t m, size_t n) -> sub_matrix_2d<detail::build_identity_type<E>, false> {
    static_assert(is_etl_expr<E>::value, "etl::sub can only be used on ETL expressions");
    static_assert(etl_traits<std::decay_t<E>>::dimensions() == 2, "Cannot use sub on vector");
    return {value, i, j, m, n};
}

/*!
 * \brief Returns view representing a sub matrix view of the given expression.
 * \param value The ETL expression
 * \param i The first index
 * \param j The second index
 * \param m The first dimension
 * \param n The second dimension
 * \return a view expression representing a sub matrix view of the given expression
 */
template <typename E>
auto sub(E&& value, size_t i, size_t j, size_t k, size_t m, size_t n, size_t o) -> sub_matrix_3d<detail::build_identity_type<E>, false> {
    static_assert(is_etl_expr<E>::value, "etl::sub can only be used on ETL expressions");
    static_assert(etl_traits<std::decay_t<E>>::dimensions() == 3, "Cannot use sub on vector");
    return {value, i, j, k, m, n, o};
}

/*!
 * \brief Returns view representing a slice view of the given expression.
 * \param value The ETL expression
 * \param first The first index
 * \param last The last index
 * \return a view expression representing a sub dimensional view of the given expression
 */
template <typename E>
auto slice(E&& value, size_t first, size_t last) -> slice_view<detail::build_identity_type<E>> {
    static_assert(is_etl_expr<E>::value, "etl::slice can only be used on ETL expressions");
    return {value, first, last};
}

/*!
 * \brief Returns view representing the reshape of another expression
 * \param value The ETL expression
 * \tparam Dims the reshape dimensions
 * \return a view expression representing the same expression with a different shape
 */
template <size_t... Dims, typename E>
auto reshape(E&& value) -> fast_matrix_view<detail::build_identity_type<E>, all_dma<E>::value, Dims...> {
    static_assert(is_etl_expr<E>::value, "etl::reshape can only be used on ETL expressions");
    cpp_assert(decay_traits<E>::is_generator || size(value) == mul_all<Dims...>::value, "Invalid size for reshape");

    return fast_matrix_view<detail::build_identity_type<E>, all_dma<E>::value, Dims...>{value};
}

/*!
 * \brief Returns view representing the reshape of another expression
 * \param value The ETL expression
 * \param sizes The dimensions of the reshaped expression
 * \return a view expression representing the same expression with a different shape
 */
template <typename E, typename... S>
auto reshape(E&& value, S... sizes) -> dyn_matrix_view<detail::build_identity_type<E>, sizeof...(sizes)> {
    static_assert(is_etl_expr<E>::value, "etl::reshape can only be used on ETL expressions");

    using ret_type = dyn_matrix_view<detail::build_identity_type<E>, sizeof...(sizes)>;

    cpp_assert(decay_traits<E>::is_generator || size(value) == util::size(sizes...), "Invalid size for reshape");

    return ret_type{value, size_t(sizes)...};
}

// Virtual Views that returns rvalues

/*!
 * \brief Returns a view representing the square magic matrix
 * \param i The size of the matrix (one side)
 * \return a virtual view expression representing the square magic matrix
 */
template <typename D = double>
auto magic(size_t i) -> detail::virtual_helper<D, magic_view<D>> {
    return detail::virtual_helper<D, magic_view<D>>{magic_view<D>{i}};
}

/*!
 * \brief Returns a view representing the square magic matrix
 * \tparam N The size of the matrix (one side)
 * \return a virtual view expression representing the square magic matrix
 */
template <size_t N, typename D = double>
auto magic() -> detail::virtual_helper<D, fast_magic_view<D, N>> {
    return detail::virtual_helper<D, fast_magic_view<D, N>>{{}};
}

} //end of namespace etl
