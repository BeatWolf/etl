//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Kernels for colum-major matrix - row-major matrix multiplication and
 * assignment to a column-major matrix
 */

#pragma once

namespace etl {

namespace impl {

namespace vec {

/*!
 * \brief Optimized version of GEMM for assignment of a small
 * Column-Major Matrix - Row Major Matrix to a Column Major Matrix.
 *
 * \param a The lhs matrix
 * \param b The rhs matrix
 * \param c The result matrix
 */
template <typename V, typename T>
void gemm_small_kernel_cr_to_c(const T* a, const T* b, T* c, size_t M, size_t N, size_t K) {
    using vec_type = V;

    static constexpr size_t vec_size = vec_type::template traits<T>::size;

    const auto i_end = M & (size_t(-vec_size));

    size_t i = 0;

    for( ; i + 3 * vec_size < i_end; i += 4 * vec_size){
        size_t j = 0;

        for (; j + 1 < N; j += 2) {
            auto r11 = vec_type::template zero<T>();
            auto r21 = vec_type::template zero<T>();
            auto r31 = vec_type::template zero<T>();
            auto r41 = vec_type::template zero<T>();

            auto r12 = vec_type::template zero<T>();
            auto r22 = vec_type::template zero<T>();
            auto r32 = vec_type::template zero<T>();
            auto r42 = vec_type::template zero<T>();

            for (size_t k = 0; k < K; ++k) {
                auto a1 = vec_type::loadu(a + i + k * M + 0 * vec_size);
                auto a2 = vec_type::loadu(a + i + k * M + 1 * vec_size);
                auto a3 = vec_type::loadu(a + i + k * M + 2 * vec_size);
                auto a4 = vec_type::loadu(a + i + k * M + 3 * vec_size);

                auto b1 = vec_type::set(b[k * N + (j + 0)]);
                auto b2 = vec_type::set(b[k * N + (j + 1)]);

                r11 = vec_type::fmadd(a1, b1, r11);
                r21 = vec_type::fmadd(a2, b1, r21);
                r31 = vec_type::fmadd(a3, b1, r31);
                r41 = vec_type::fmadd(a4, b1, r41);

                r12 = vec_type::fmadd(a1, b2, r12);
                r22 = vec_type::fmadd(a2, b2, r22);
                r32 = vec_type::fmadd(a3, b2, r32);
                r42 = vec_type::fmadd(a4, b2, r42);
            }

            vec_type::storeu(c + i + (j + 0) * M + 0 * vec_size, r11);
            vec_type::storeu(c + i + (j + 0) * M + 1 * vec_size, r21);
            vec_type::storeu(c + i + (j + 0) * M + 2 * vec_size, r31);
            vec_type::storeu(c + i + (j + 0) * M + 3 * vec_size, r41);

            vec_type::storeu(c + i + (j + 1) * M + 0 * vec_size, r12);
            vec_type::storeu(c + i + (j + 1) * M + 1 * vec_size, r22);
            vec_type::storeu(c + i + (j + 1) * M + 2 * vec_size, r32);
            vec_type::storeu(c + i + (j + 1) * M + 3 * vec_size, r42);
        }

        if (j < N) {
            auto r11 = vec_type::template zero<T>();
            auto r21 = vec_type::template zero<T>();
            auto r31 = vec_type::template zero<T>();
            auto r41 = vec_type::template zero<T>();

            for (size_t k = 0; k < K; ++k) {
                auto a1 = vec_type::loadu(a + i + k * M + 0 * vec_size);
                auto a2 = vec_type::loadu(a + i + k * M + 1 * vec_size);
                auto a3 = vec_type::loadu(a + i + k * M + 2 * vec_size);
                auto a4 = vec_type::loadu(a + i + k * M + 3 * vec_size);

                auto b1 = vec_type::set(b[k * N + j]);

                r11 = vec_type::fmadd(a1, b1, r11);
                r21 = vec_type::fmadd(a2, b1, r21);
                r31 = vec_type::fmadd(a3, b1, r31);
                r41 = vec_type::fmadd(a4, b1, r41);
            }

            vec_type::storeu(c + i + j * M + 0 * vec_size, r11);
            vec_type::storeu(c + i + j * M + 1 * vec_size, r21);
            vec_type::storeu(c + i + j * M + 2 * vec_size, r31);
            vec_type::storeu(c + i + j * M + 3 * vec_size, r41);
        }
    }

    for( ; i + 1 * vec_size < i_end; i += 2 * vec_size){
        size_t j = 0;

        for (; j + 1 < N; j += 2) {
            auto r11 = vec_type::template zero<T>();
            auto r21 = vec_type::template zero<T>();

            auto r12 = vec_type::template zero<T>();
            auto r22 = vec_type::template zero<T>();

            for (size_t k = 0; k < K; ++k) {
                auto a1 = vec_type::loadu(a + i + k * M + 0 * vec_size);
                auto a2 = vec_type::loadu(a + i + k * M + 1 * vec_size);

                auto b1 = vec_type::set(b[k * N + (j + 0)]);
                auto b2 = vec_type::set(b[k * N + (j + 1)]);

                r11 = vec_type::fmadd(a1, b1, r11);
                r21 = vec_type::fmadd(a2, b1, r21);

                r12 = vec_type::fmadd(a1, b2, r12);
                r22 = vec_type::fmadd(a2, b2, r22);
            }

            vec_type::storeu(c + i + (j + 0) * M + 0 * vec_size, r11);
            vec_type::storeu(c + i + (j + 0) * M + 1 * vec_size, r21);

            vec_type::storeu(c + i + (j + 1) * M + 0 * vec_size, r12);
            vec_type::storeu(c + i + (j + 1) * M + 1 * vec_size, r22);
        }

        if (j < N) {
            auto r11 = vec_type::template zero<T>();
            auto r21 = vec_type::template zero<T>();

            for (size_t k = 0; k < K; ++k) {
                auto a1 = vec_type::loadu(a + i + k * M + 0 * vec_size);
                auto a2 = vec_type::loadu(a + i + k * M + 1 * vec_size);

                auto b1 = vec_type::set(b[k * N + j]);

                r11 = vec_type::fmadd(a1, b1, r11);
                r21 = vec_type::fmadd(a2, b1, r21);
            }

            vec_type::storeu(c + i + j * M + 0 * vec_size, r11);
            vec_type::storeu(c + i + j * M + 1 * vec_size, r21);
        }
    }

    for( ; i < i_end; i += vec_size){
        size_t j = 0;

        for (; j + 1 < N; j += 2) {
            auto r11 = vec_type::template zero<T>();
            auto r12 = vec_type::template zero<T>();

            for (size_t k = 0; k < K; ++k) {
                auto a1 = vec_type::loadu(a + i + k * M + 0 * vec_size);

                auto b1 = vec_type::set(b[k * N + (j + 0)]);
                auto b2 = vec_type::set(b[k * N + (j + 1)]);

                r11 = vec_type::fmadd(a1, b1, r11);
                r12 = vec_type::fmadd(a1, b2, r12);
            }

            vec_type::storeu(c + i + (j + 0) * M, r11);
            vec_type::storeu(c + i + (j + 1) * M, r12);
        }

        if (j < N) {
            auto r11 = vec_type::template zero<T>();

            for (size_t k = 0; k < K; ++k) {
                auto a1 = vec_type::loadu(a + i + k * M + 0 * vec_size);

                auto b1 = vec_type::set(b[k * N + j]);

                r11 = vec_type::fmadd(a1, b1, r11);
            }

            vec_type::storeu(c + i + j * M, r11);
        }
    }

    for( ; i < M; ++i){
        size_t j = 0;

        for (; j + 1 < N; j += 2) {
            auto r11 = T();
            auto r12 = T();

            for (size_t k = 0; k < K; ++k) {
                r11 += a[i + k * M] * b[k * N + (j + 0)];
                r12 += a[i + k * M] * b[k * N + (j + 1)];
            }

            c[i + (j + 0) * M] = r11;
            c[i + (j + 1) * M] = r12;
        }

        if (j < N) {
            auto r11 = T();

            for (size_t k = 0; k < K; ++k) {
                r11 += a[i + k * M] * b[k * N + j];
            }

            c[i + j * M] = r11;
        }
    }
}

/*!
 * \brief Optimized version of GEMM for assignment of a large
 * Column-Major Matrix - Row Major Matrix to a Column Major Matrix.
 *
 * \param a The lhs matrix
 * \param b The rhs matrix
 * \param c The result matrix
 */
template <typename V, typename T>
void gemm_large_kernel_cr_to_c(const T* a, const T* b, T* c, size_t M, size_t N, size_t K) {
    cpp_unused(a);
    cpp_unused(b);
    cpp_unused(c);
    cpp_unused(M);
    cpp_unused(N);
    cpp_unused(K);
}

/*!
 * \brief Vectorized implementation of column-major matrix - row-major matrix
 * multiplication and assignment into a column-major matrix.
 *
 * \param a The lhs matrix
 * \param b The rhs matrix
 * \param c The result matrix
 *
 * \param M The number of rows of the matrix A and rows of the matrix C
 * \param N The number of columns of the matrix B and columns of the matrix C
 * \param K The number of columns of the matrix A and rows of the matrix B
 */
template <typename T>
void gemm_cr_to_c(const T* a, const T* b, T* c, size_t M, size_t N, size_t K) {
    cpp_assert(vec_enabled, "At least one vector mode must be enabled for impl::VEC");

    if (M * N <= gemm_rr_small_threshold) {
        gemm_small_kernel_cr_to_c<default_vec>(a, b, c, M, N, K);
    } else {
        direct_fill_n(c, M * N, T(0));
        gemm_large_kernel_cr_to_c<default_vec>(a, b, c, M, N, K);
    }
}

} //end of namespace vec
} //end of namespace impl
} //end of namespace etl
