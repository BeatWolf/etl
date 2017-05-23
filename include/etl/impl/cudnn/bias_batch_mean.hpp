//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief bias_batch_mean implementations with NVidia cuDNN library
 */

#pragma once

#ifdef ETL_CUDNN_MODE

#include "etl/impl/cublas/cuda.hpp"
#include "etl/impl/cudnn/cudnn.hpp"

#endif

namespace etl {

namespace impl {

namespace cudnn {

#ifdef ETL_CUDNN_MODE

/*!
 * \brief CUDNN Implementation of the bias_batch_mean operation
 * \param x The input tensor
 * \param y The output tensor
 */
template <typename X, typename Y>
void bias_batch_mean_4d(X&& x, Y&& y) {
    using type = value_t<X>;

    type alpha[] = {1.0f};
    type beta[] = {0.0f};

    decltype(auto) handle = start_cudnn();

    // Prepare the tensors
    auto x_tensor = create_tensor(x);
    auto y_tensor = create_tensor(y);

    // Allocate GPU memory, if necessary

    x.ensure_gpu_up_to_date();
    y.ensure_gpu_allocated();

    // Perform the convolution

    cudnn_check(cudnnConvolutionBackwardBias(handle.get(),
        alpha, *x_tensor, x.gpu_memory(),
        beta, *y_tensor, y.gpu_memory()));

    y.validate_gpu();
    y.invalidate_cpu();
}

/*!
 * \brief CUDNN Implementation of the bias_batch_mean operation
 * \param x The input tensor
 * \param y The output tensor
 */
template <typename X, typename Y>
void bias_batch_mean_2d(X&& x, Y&& y) {
    using type = value_t<X>;

    type alpha[] = {1.0f};
    type beta[] = {0.0f};

    decltype(auto) handle = start_cudnn();

    // Prepare the tensors
    auto x_tensor = create_tensor_front(x);
    auto y_tensor = create_tensor(y);

    // Allocate GPU memory, if necessary

    x.ensure_gpu_up_to_date();
    y.ensure_gpu_allocated();

    // Perform the convolution

    cudnn_check(cudnnConvolutionBackwardBias(handle.get(),
        alpha, *x_tensor, x.gpu_memory(),
        beta, *y_tensor, y.gpu_memory()));

    y.validate_gpu();
    y.invalidate_cpu();
}

#else

//COVERAGE_EXCLUDE_BEGIN

/*!
 * \brief CUDNN Implementation of the bias_batch_mean operation
 * \param x The input tensor
 * \param y The output tensor
 */
template <typename X, typename Y>
void bias_batch_mean_4d(X&& x, Y&& y) {
    cpp_unused(x);
    cpp_unused(y);
}

/*!
 * \brief CUDNN Implementation of the bias_batch_mean operation
 * \param x The input tensor
 * \param y The output tensor
 */
template <typename X, typename Y>
void bias_batch_mean_2d(X&& x, Y&& y) {
    cpp_unused(x);
    cpp_unused(y);
}

//COVERAGE_EXCLUDE_END

#endif

} //end of namespace cudnn

} //end of namespace impl

} //end of namespace etl
