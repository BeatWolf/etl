//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

#ifdef ETL_CUBLAS_MODE

#include "etl/impl/cublas/cuda.hpp"
#include "etl/impl/cublas/cublas.hpp"

#endif

namespace etl {

namespace impl {

namespace cublas {

#ifdef ETL_CUBLAS_MODE

using cfloat  = std::complex<float>;  ///< Complex float type
using cdouble = std::complex<double>; ///< Complex double type

/*!
 * \brief GPU Transpose (single-precision)
 * \param handle The cublas handle
 * \param transa The operation on a
 * \param transb The operation on b
 * \param m The first dimension of a
 * \param n The second dimension of a
 * \param alpha The multiplicator of a
 * \param A A pointer to a's memory
 * \param lda The leading dimension of a
 * \param beta The multiplicator of b
 * \param B A pointer to b's memory
 * \param ldb The leading dimension of b
 * \param C A pointer to c's memory
 * \param ldc The leading dimension of c
 */
inline void cublas_geam(cublasHandle_t handle, cublasOperation_t transa, cublasOperation_t transb, size_t m, size_t n,
                        const float* alpha, const float* A, size_t lda, const float* beta, const float* B, size_t ldb, float* C, size_t ldc) {
    cublas_check(cublasSgeam(handle, transa, transb, m, n, alpha, A, lda, beta, B, ldb, C, ldc));
}

/*!
 * \brief GPU Transpose (double-precision)
 * \param handle The cublas handle
 * \param transa The operation on a
 * \param transb The operation on b
 * \param m The first dimension of a
 * \param n The second dimension of a
 * \param alpha The multiplicator of a
 * \param A A pointer to a's memory
 * \param lda The leading dimension of a
 * \param beta The multiplicator of b
 * \param B A pointer to b's memory
 * \param ldb The leading dimension of b
 * \param C A pointer to c's memory
 * \param ldc The leading dimension of c
 */
inline void cublas_geam(cublasHandle_t handle, cublasOperation_t transa, cublasOperation_t transb, size_t m, size_t n,
                        const double* alpha, const double* A, size_t lda, const double* beta, const double* B, size_t ldb, double* C, size_t ldc) {
    cublas_check(cublasDgeam(handle, transa, transb, m, n, alpha, A, lda, beta, B, ldb, C, ldc));
}

/*!
 * \brief GPU Transpose (complex-single-precision)
 * \param handle The cublas handle
 * \param transa The operation on a
 * \param transb The operation on b
 * \param m The first dimension of a
 * \param n The second dimension of a
 * \param alpha The multiplicator of a
 * \param A A pointer to a's memory
 * \param lda The leading dimension of a
 * \param beta The multiplicator of b
 * \param B A pointer to b's memory
 * \param ldb The leading dimension of b
 * \param C A pointer to c's memory
 * \param ldc The leading dimension of c
 */
inline void cublas_geam(cublasHandle_t handle, cublasOperation_t transa, cublasOperation_t transb, size_t m, size_t n,
                        const cfloat* alpha, const cfloat* A, size_t lda, const cfloat* beta, const cfloat* B, size_t ldb, cfloat* C, size_t ldc) {
    cublas_check(cublasCgeam(handle, transa, transb, m, n,
                reinterpret_cast<const cuComplex*>(alpha), reinterpret_cast<const cuComplex*>(A), lda,
                reinterpret_cast<const cuComplex*>(beta), reinterpret_cast<const cuComplex*>(B), ldb,
                reinterpret_cast<cuComplex*>(C), ldc));
}

/*!
 * \brief GPU Transpose (complex-double-precision)
 * \param handle The cublas handle
 * \param transa The operation on a
 * \param transb The operation on b
 * \param m The first dimension of a
 * \param n The second dimension of a
 * \param alpha The multiplicator of a
 * \param A A pointer to a's memory
 * \param lda The leading dimension of a
 * \param beta The multiplicator of b
 * \param B A pointer to b's memory
 * \param ldb The leading dimension of b
 * \param C A pointer to c's memory
 * \param ldc The leading dimension of c
 */
inline void cublas_geam(cublasHandle_t handle, cublasOperation_t transa, cublasOperation_t transb, size_t m, size_t n,
                        const cdouble* alpha, const cdouble* A, size_t lda, const cdouble* beta,
                        const cdouble* B, size_t ldb, cdouble* C, size_t ldc) {
    cublas_check(cublasZgeam(handle, transa, transb, m, n,
                reinterpret_cast<const cuDoubleComplex*>(alpha), reinterpret_cast<const cuDoubleComplex*>(A), lda,
                reinterpret_cast<const cuDoubleComplex*>(beta), reinterpret_cast<const cuDoubleComplex*>(B), ldb,
                reinterpret_cast<cuDoubleComplex*>(C), ldc));
}

/*!
 * \brief Inplace transposition of the square matrix c
 * \param c The matrix to transpose
 */
template <typename C, cpp_enable_if(all_dma<C>::value&& all_floating<C>::value)>
void inplace_square_transpose(C&& c) {
    decltype(auto) handle = start_cublas();

    using T = value_t<C>;

    static constexpr bool row_major = decay_traits<C>::storage_order == order::RowMajor;

    auto alpha = T(1);
    auto beta  = T(0);

    auto a_gpu = cuda::cuda_allocate_only<T>(etl::size(c));

    auto c_gpu = c.direct();
    c_gpu.gpu_allocate_copy_if_necessary();

    cuda_check(cudaMemcpy(a_gpu.get(), c_gpu.gpu_memory(), etl::size(c) * sizeof(T), cudaMemcpyDeviceToDevice));

    if(row_major){
        cublas_geam(handle.get(), CUBLAS_OP_T, CUBLAS_OP_T, etl::dim<0>(c), etl::dim<1>(c), &alpha, a_gpu.get(), etl::dim<1>(c), &beta, a_gpu.get(), etl::dim<1>(c), c_gpu.gpu_memory(), etl::dim<0>(c));
    } else {
        cublas_geam(handle.get(), CUBLAS_OP_T, CUBLAS_OP_T, etl::dim<0>(c), etl::dim<1>(c), &alpha, a_gpu.get(), etl::dim<0>(c), &beta, a_gpu.get(), etl::dim<0>(c), c_gpu.gpu_memory(), etl::dim<1>(c));
    }
}

/*!
 * \brief Inplace transposition of the rectangular matrix c
 * \param c The matrix to transpose
 */
template <typename C, cpp_enable_if(all_dma<C>::value&& all_floating<C>::value)>
void inplace_rectangular_transpose(C&& c) {
    inplace_square_transpose(c);
}

/*!
 * \brief Transpose the matrix a and the store the result in c
 * \param a The matrix to transpose
 * \param c The target matrix
 */
template <typename A, typename C, cpp_enable_if(all_dma<A, C>::value&& all_floating<A, C>::value)>
void transpose(A&& a, C&& c) {
    decltype(auto) handle = start_cublas();

    static constexpr bool row_major = decay_traits<A>::storage_order == order::RowMajor;

    auto alpha = value_t<A>(1.0);
    auto beta  = value_t<A>(0.0);

    auto a_gpu = a.direct();
    auto c_gpu = c.direct();

    a_gpu.gpu_allocate_copy_if_necessary();
    c_gpu.gpu_allocate_if_necessary();

    if(row_major){
        cublas_geam(handle.get(), CUBLAS_OP_T, CUBLAS_OP_T, etl::dim<0>(a), etl::dim<1>(a), &alpha, a_gpu.gpu_memory(), etl::dim<1>(a), &beta, a_gpu.gpu_memory(), etl::dim<1>(a), c_gpu.gpu_memory(), etl::dim<1>(c));
    } else {
        cublas_geam(handle.get(), CUBLAS_OP_T, CUBLAS_OP_T, etl::dim<0>(a), etl::dim<1>(a), &alpha, a_gpu.gpu_memory(), etl::dim<0>(a), &beta, a_gpu.gpu_memory(), etl::dim<0>(a), c_gpu.gpu_memory(), etl::dim<0>(c));
    }
}

/*!
 * \brief Inplace transposition of the square matrix c
 * \param c The matrix to transpose
 */
template <typename C, cpp_disable_if(all_dma<C>::value&& all_floating<C>::value)>
void inplace_square_transpose(C&& /*c*/) {}

/*!
 * \brief Inplace transposition of the rectangular matrix c
 * \param c The matrix to transpose
 */ template <typename C, cpp_disable_if(all_dma<C>::value&& all_floating<C>::value)>
void inplace_rectangular_transpose(C&& /*c*/) {}

/*!
 * \brief Transpose the matrix a and the store the result in c
 * \param a The matrix to transpose
 * \param c The target matrix
 */
template <typename A, typename C, cpp_disable_if(all_dma<A, C>::value&& all_floating<A, C>::value)>
void transpose(A&& /*a*/, C&& /*c*/) {}

#else

//COVERAGE_EXCLUDE_BEGIN

/*!
 * \brief Inplace transposition of the square matrix c
 * \param c The matrix to transpose
 */
template <typename C>
void inplace_square_transpose(C&& c) {
    cpp_unused(c);
    cpp_unreachable("CUBLAS not enabled/available");
}

/*!
 * \brief Inplace transposition of the rectangular matrix c
 * \param c The matrix to transpose
 */
template <typename C>
void inplace_rectangular_transpose(C&& c) {
    cpp_unused(c);
    cpp_unreachable("CUBLAS not enabled/available");
}

/*!
 * \brief Transpose the matrix a and the store the result in c
 * \param a The matrix to transpose
 * \param c The target matrix
 */
template <typename A, typename C>
void transpose(A&& a, C&& c) {
    cpp_unused(a);
    cpp_unused(c);
    cpp_unreachable("CUBLAS not enabled/available");
}

//COVERAGE_EXCLUDE_END

#endif

} //end of namespace cublas

} //end of namespace impl

} //end of namespace etl
