//=======================================================================
// Copyright (c) 2014-2018 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief EGBLAS wrappers for the not_equal operation.
 */

#pragma once

#ifdef ETL_EGBLAS_MODE

#include "etl/impl/cublas/cuda.hpp"

#include <egblas.hpp>

#endif

namespace etl::impl::egblas {

/*!
 * \brief Indicates if EGBLAS has single-precision not_equal.
 */
#ifdef EGBLAS_HAS_SNOT_EQUAL
static constexpr bool has_snot_equal = true;
#else
static constexpr bool has_snot_equal = false;
#endif

/*!
 * \brief Wrappers for single-precision egblas not_equal operation
 * \param n The size of the vector
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 * \param C The memory of the vector c
 * \param ldc The leading dimension of c
 */
inline void not_equal(size_t n, const float* A, size_t lda, const float* B, size_t ldb, bool* C, size_t ldc) {
#ifdef EGBLAS_HAS_SNOT_EQUAL
    inc_counter("egblas");
    egblas_snot_equal(n, A, lda, B, ldb, C, ldc);
#else
    cpp_unused(n);
    cpp_unused(A);
    cpp_unused(lda);
    cpp_unused(B);
    cpp_unused(ldb);
    cpp_unused(C);
    cpp_unused(ldc);

    cpp_unreachable("Invalid call to egblas::not_equal");
#endif
}

/*!
 * \brief Indicates if EGBLAS has double-precision not_equal.
 */
#ifdef EGBLAS_HAS_DNOT_EQUAL
static constexpr bool has_dnot_equal = true;
#else
static constexpr bool has_dnot_equal = false;
#endif

/*!
 * \brief Wrappers for double-precision egblas not_equal operation
 * \param n The size of the vector
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 * \param C The memory of the vector c
 * \param ldc The leading dimension of c
 */
inline void not_equal(size_t n, const double* A, size_t lda, const double* B, size_t ldb, bool* C, size_t ldc) {
#ifdef EGBLAS_HAS_DNOT_EQUAL
    inc_counter("egblas");
    egblas_dnot_equal(n, A, lda, B, ldb, C, ldc);
#else
    cpp_unused(n);
    cpp_unused(A);
    cpp_unused(lda);
    cpp_unused(B);
    cpp_unused(ldb);
    cpp_unused(C);
    cpp_unused(ldc);

    cpp_unreachable("Invalid call to egblas::not_equal");
#endif
}

/*!
 * \brief Indicates if EGBLAS has complex single-precision not_equal.
 */
#ifdef EGBLAS_HAS_CNOT_EQUAL
static constexpr bool has_cnot_equal = true;
#else
static constexpr bool has_cnot_equal = false;
#endif

/*!
 * \brief Wrappers for complex single-precision egblas not_equal operation
 * \param n The size of the vector
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 * \param C The memory of the vector c
 * \param ldc The leading dimension of c
 */
inline void not_equal(size_t n, const std::complex<float>* A, size_t lda, const std::complex<float>* B, size_t ldb, bool* C, size_t ldc) {
#ifdef EGBLAS_HAS_CNOT_EQUAL
    inc_counter("egblas");
    egblas_cnot_equal(n, reinterpret_cast<const cuComplex*>(A), lda, reinterpret_cast<const cuComplex*>(B), ldb, C, ldc);
#else
    cpp_unused(n);
    cpp_unused(A);
    cpp_unused(lda);
    cpp_unused(B);
    cpp_unused(ldb);
    cpp_unused(C);
    cpp_unused(ldc);

    cpp_unreachable("Invalid call to egblas::not_equal");
#endif
}

/*!
 * \brief Wrappers for complex single-precision egblas not_equal operation
 * \param n The size of the vector
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 * \param C The memory of the vector c
 * \param ldc The leading dimension of c
 */
inline void not_equal(size_t n, const etl::complex<float>* A, size_t lda, const etl::complex<float>* B, size_t ldb, bool* C, size_t ldc) {
#ifdef EGBLAS_HAS_CNOT_EQUAL
    inc_counter("egblas");
    egblas_cnot_equal(n, reinterpret_cast<const cuComplex*>(A), lda, reinterpret_cast<const cuComplex*>(B), ldb, C, ldc);
#else
    cpp_unused(n);
    cpp_unused(A);
    cpp_unused(lda);
    cpp_unused(B);
    cpp_unused(ldb);
    cpp_unused(C);
    cpp_unused(ldc);

    cpp_unreachable("Invalid call to egblas::not_equal");
#endif
}

/*!
 * \brief Indicates if EGBLAS has complex double-precision not_equal.
 */
#ifdef EGBLAS_HAS_ZNOT_EQUAL
static constexpr bool has_znot_equal = true;
#else
static constexpr bool has_znot_equal = false;
#endif

/*!
 * \brief Wrappers for complex double-precision egblas not_equal operation
 * \param n The size of the vector
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 * \param C The memory of the vector c
 * \param ldc The leading dimension of c
 */
inline void not_equal(size_t n, const std::complex<double>* A, size_t lda, const std::complex<double>* B, size_t ldb, bool* C, size_t ldc) {
#ifdef EGBLAS_HAS_ZNOT_EQUAL
    inc_counter("egblas");
    egblas_znot_equal(n, reinterpret_cast<const cuDoubleComplex*>(A), lda, reinterpret_cast<const cuDoubleComplex*>(B), ldb, C, ldc);
#else
    cpp_unused(n);
    cpp_unused(A);
    cpp_unused(lda);
    cpp_unused(B);
    cpp_unused(ldb);
    cpp_unused(C);
    cpp_unused(ldc);

    cpp_unreachable("Invalid call to egblas::not_equal");
#endif
}

/*!
 * \brief Wrappers for complex double-precision egblas not_equal operation
 * \param n The size of the vector
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 * \param C The memory of the vector c
 * \param ldc The leading dimension of c
 */
inline void not_equal(size_t n, const etl::complex<double>* A, size_t lda, const etl::complex<double>* B, size_t ldb, bool* C, size_t ldc) {
#ifdef EGBLAS_HAS_ZNOT_EQUAL
    inc_counter("egblas");
    egblas_znot_equal(n, reinterpret_cast<const cuDoubleComplex*>(A), lda, reinterpret_cast<const cuDoubleComplex*>(B), ldb, C, ldc);
#else
    cpp_unused(n);
    cpp_unused(A);
    cpp_unused(lda);
    cpp_unused(B);
    cpp_unused(ldb);
    cpp_unused(C);
    cpp_unused(ldc);

    cpp_unreachable("Invalid call to egblas::not_equal");
#endif
}

} //end of namespace etl::impl::egblas
