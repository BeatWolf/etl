//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Standard implementation of the decompositions
 */

#pragma once

namespace etl {

namespace impl {

namespace standard {

/*!
 * \brief Performs the PA=LU decomposition of the matrix A
 * \param A The matrix to decompose
 * \param L The resulting L matrix
 * \param U The resulting U matrix
 * \param P The resulting P matrix
 */
template <typename AT, typename LT, typename UT, typename PT>
void lu(const AT& A, LT& L, UT& U, PT& P) {
    const auto n = etl::dim(A, 0);

    L = 0;
    U = 0;
    P = 0;

    // 1. Create the pivot matrix

    for(std::size_t i = 0; i < n; ++i){
        P(i, i) = 1;
    }

    for(std::size_t i = 0; i < n; ++i){
        auto max_j = i;

        for(std::size_t j = i; j < n; ++j){
            if(std::abs(A(j, i)) > A(max_j, i)){
                max_j = j;
            }
        }

        if(max_j != i){
            for(std::size_t k = 0; k < n; ++k){
                using std::swap;
                swap(P(i, k), P(max_j, k));
            }
        }
    }

    auto Ap = etl::force_temporary(P * A);

    for(std::size_t i = 0; i < n; ++i){
        L(i, i) = 1;
    }

    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (j <= i) {
                value_t<AT> s = 0;
                for (std::size_t k = 0; k < j; ++k) {
                    s += L(j, k) * U(k, i);
                }

                U(j, i) = Ap(j, i) - s;
            }

            if (j >= i) {
                value_t<AT> s = 0;
                for (std::size_t k = 0; k < i; ++k) {
                    s += L(j, k) * U(k, i);
                }

                L(j, i) = (Ap(j, i) - s) / U(i, i);
            }
        }
    }
}

/*!
 * \brief Use the householder algorithm to perform the A=QR decomposition of the matrix A
 * \param A The matrix to decompose
 * \param Q The resulting Q matrix
 * \param R The resulting R matrix
 */
template <typename AT, typename QT, typename RT>
void householder(const AT& A, QT& Q, RT& R) {
    using T = value_t<AT>;

    const auto m = etl::dim<0>(A);
    const auto n = etl::dim<1>(A);

    std::vector<QT> q(m);

    etl::dyn_matrix<T> z(m, n);
    z = A;

    for (size_t k = 0; k < n && k < m - 1; k++) {
        etl::dyn_matrix<T> zz(m, n, T(0));

        for(size_t i = 0; i < k; ++i){
            zz(i, i) = 1;
        }

        for(size_t i = k; i < m; ++i){
            for(size_t j = k; j < n; ++j){
                zz(i, j) = z(i, j);
            }
        }

        z = std::move(zz);

        // x -> Take k-th column of z
        etl::dyn_vector<T> x(m);
        for(size_t i = 0; i < m; ++i){
            x[i] = z(i, k);
        }

        auto a = norm(x);
        if (A(k, k) > 0) {
            a = -a;
        }

        etl::dyn_vector<T> e(m);
        for (size_t i = 0; i < m; i++){
            e[i] = x[i] + ((i == k) ? a : 0);
        }

        e /= norm(e);

        for(size_t i = 0; i < m; ++i){
            for(size_t j = 0; j < m; ++j){
                q[k](i, j) = -2 * e[i] * e[j];
            }

            q[k](i, i) += 1;
        }

        z = q[k] * z;
    }

    Q = q[0];
    R = q[0] * A;

    for (size_t i = 1; i < n && i < m - 1; i++) {
        etl::dyn_matrix<T> z1(m, m);
        z1 = q[i] * Q;
        Q = std::move(z1);
    }

    R = Q * A;

    Q = transpose(Q);
}

/*!
 * \brief Performs the A=QR decomposition of the matrix A
 * \param A The matrix to decompose
 * \param L The resulting L matrix
 * \param U The resulting U matrix
 * \param P The resulting P matrix
 */
template <typename AT, typename QT, typename RT>
void qr(const AT& A, QT& Q, RT& R) {
    householder(A, Q, R);
}

} //end of namespace standard
} //end of namespace impl
} //end of namespace etl
