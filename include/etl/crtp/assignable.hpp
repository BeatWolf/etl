//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

#include "etl/impl/transpose.hpp"
#include "etl/impl/fft.hpp"

/*!
 * \file
 * \brief Use CRTP technique to inject assign operations into expressions and value classes.
 */

namespace etl {

/*!
 * \brief CRTP class to inject assign operations to matrix and vector structures.
 */
template <typename D, typename V>
struct assignable {
    using derived_t  = D; ///< The derived type
    using value_type = V; ///< The value type

    /*!
     * \brief Assign the given expression to the unary expression
     * \param e The expression to get the values from
     * \return the unary expression
     */
    template <typename E, cpp_enable_iff(is_etl_expr<E>)>
    derived_t& operator=(E&& e) {
        validate_assign(as_derived(), e);
        e.assign_to(as_derived());
        return as_derived();
    }

    /*!
     * \brief Assign the given expression to the unary expression
     * \param v The expression to get the values from
     * \return the unary expression
     */
    derived_t& operator=(const value_type& v) {
        std::fill(as_derived().begin(), as_derived().end(), v);
        return as_derived();
    }

    /*!
     * \brief Assign the given container to the unary expression
     * \param vec The container to get the values from
     * \return the unary expression
     */
    template <typename Container, cpp_enable_iff(!is_etl_expr<Container> && std::is_convertible<typename Container::value_type, value_type>::value)>
    derived_t& operator=(const Container& vec) {
        validate_assign(as_derived(), vec);

        std::copy(vec.begin(), vec.end(), as_derived().begin());

        return as_derived();
    }

private:
    /*!
     * \brief Returns a reference to the derived object, i.e. the object using the CRTP injector.
     * \return a reference to the derived object.
     */
    derived_t& as_derived() noexcept {
        return *static_cast<derived_t*>(this);
    }
};

} //end of namespace etl
