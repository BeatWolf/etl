//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief EGBLAS wrappers for the cce operations.
 */

#pragma once

#ifdef ETL_EGBLAS_MODE

#include "etl/impl/cublas/cuda.hpp"

#include <egblas.hpp>

#endif

namespace etl {

namespace impl {

namespace egblas {

/*!
 * \brief Indicates if EGBLAS has single-precision CCE loss
 */
#ifdef EGBLAS_HAS_CCE_SLOSS
static constexpr bool has_cce_sloss = true;
#else
static constexpr bool has_cce_sloss = false;
#endif

/*!
 * \brief Wrappers for single-precision egblas log operation
 * \param n The size of the vector
 * \param alpha The scaling factor alpha
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 */
inline float cce_loss(size_t n, float* alpha, float* A , size_t lda, float* B , size_t ldb){
#ifdef EGBLAS_HAS_CCE_SLOSS
    return egblas_cce_sloss(n, *alpha, A, lda, B, ldb);
#else
    cpp_unused(n);
    cpp_unused(alpha);
    cpp_unused(A);
    cpp_unused(lda);
    cpp_unused(B);
    cpp_unused(ldb);

    cpp_unreachable("Invalid call to egblas::cce_loss");

    return 0.0;
#endif
}

/*!
 * \brief Indicates if EGBLAS has double-precision CCE loss
 */
#ifdef EGBLAS_HAS_CCE_DLOSS
static constexpr bool has_cce_dloss = true;
#else
static constexpr bool has_cce_dloss = false;
#endif

/*!
 * \brief Wrappers for double-precision egblas log operation
 * \param n The size of the vector
 * \param alpha The scaling factor alpha
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 */
inline double cce_loss(size_t n, double* alpha, double* A , size_t lda, double* B , size_t ldb){
#ifdef EGBLAS_HAS_CCE_DLOSS
    return egblas_cce_dloss(n, *alpha, A, lda, B, ldb);
#else
    cpp_unused(n);
    cpp_unused(alpha);
    cpp_unused(A);
    cpp_unused(lda);
    cpp_unused(B);
    cpp_unused(ldb);

    cpp_unreachable("Invalid call to egblas::cce_loss");

    return 0.0;
#endif
}

/*!
 * \brief Indicates if EGBLAS has single-precision CCE error
 */
#ifdef EGBLAS_HAS_CCE_SERROR
static constexpr bool has_cce_serror = true;
#else
static constexpr bool has_cce_serror = false;
#endif

/*!
 * \brief Wrappers for single-precision egblas log operation
 * \param n The size of the vector
 * \param alpha The scaling factor alpha
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 */
inline float cce_error(size_t n, size_t m, float* alpha, float* A , float* B){
#ifdef EGBLAS_HAS_CCE_SERROR
    return egblas_cce_serror(n, m, *alpha, A, B);
#else
    cpp_unused(n);
    cpp_unused(m);
    cpp_unused(alpha);
    cpp_unused(A);
    cpp_unused(B);

    cpp_unreachable("Invalid call to egblas::cce_error");

    return 0.0;
#endif
}

/*!
 * \brief Indicates if EGBLAS has double-precision CCE error
 */
#ifdef EGBLAS_HAS_CCE_DERROR
static constexpr bool has_cce_derror = true;
#else
static constexpr bool has_cce_derror = false;
#endif

/*!
 * \brief Wrappers for double-precision egblas log operation
 * \param n The size of the vector
 * \param alpha The scaling factor alpha
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 */
inline double cce_error(size_t n, size_t m, double* alpha, double* A , double* B){
#ifdef EGBLAS_HAS_CCE_DERROR
    return egblas_cce_derror(n, m, *alpha, A, B);
#else
    cpp_unused(n);
    cpp_unused(m);
    cpp_unused(alpha);
    cpp_unused(A);
    cpp_unused(B);

    cpp_unreachable("Invalid call to egblas::cce_error");

    return 0.0;
#endif
}

} //end of namespace egblas
} //end of namespace impl
} //end of namespace etl
