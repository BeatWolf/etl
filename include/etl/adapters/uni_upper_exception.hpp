//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains uni upper triangular matrix exception implementation
 */

#pragma once

#include <exception>

namespace etl {

/*!
 * \brief Exception that is thrown when an operation is made to
 * a uni upper triangular matrix that would render it non-uni upper
 * triangular.
 */
struct uni_upper_exception : std::exception {
    /*!
     * \brief Returns a description of the exception
     */
    virtual const char* what() const noexcept {
        return "Invalid assignment to a uni upper triangular matrix";
    }
};

} //end of namespace etl