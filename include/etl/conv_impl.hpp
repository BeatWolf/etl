//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Enumeration of the different convolution implementations
 */

#pragma once

namespace etl {

/*!
 * \brief Enumeration describing the different convolution implementations
 */
enum class conv_impl {
    STD, ///< Standard implementation
    SSE, ///< Vectorized SSE implementation
    AVX  ///< Vectorized AVX implementation
};

/*!
 * \brief Enumeration describing the different multiple convolution implementations
 */
enum class conv_multi_impl {
    STD, ///< Standard implementation
    FFT, ///< Reductiont too FFT
    BLAS ///< Reduction to BLAS (GEMM)
};

} //end of namespace etl
