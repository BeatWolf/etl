//=======================================================================
// Copyright (c) 2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef ETL_ITERABLE_HPP
#define ETL_ITERABLE_HPP

#include <algorithm>

/*
 * Use CRTP technique to inject functions that test the values of
 * the expressions or the value classes.
 */

namespace etl {

//TODO The name of this CRTP class is not very good

template<typename D>
struct iterable {
    using derived_t = D;

    derived_t& as_derived() noexcept {
        return *static_cast<derived_t*>(this);
    }

    const derived_t& as_derived() const noexcept {
        return *static_cast<const derived_t*>(this);
    }

    bool is_finite() const noexcept {
        return std::all_of(as_derived().begin(), as_derived().end(), static_cast<bool(*)(value_t<derived_t>)>(std::isfinite));
    }

    bool is_zero() const noexcept {
        return std::all_of(as_derived().begin(), as_derived().end(), [](value_t<derived_t> v){ return v == value_t<derived_t>(0); });;
    }

    //TODO The following operations could probably be moved away

    bool is_square() const noexcept {
        cpp_assert(decay_traits<derived_t>::dimensions() == 2, "Only 2D matrix can be square or rectangular");
        return etl::dim<0>(as_derived()) == etl::dim<1>(as_derived());
    }

    bool is_rectangular() const noexcept {
        return !is_square();
    }

    bool is_sub_square() const noexcept {
        cpp_assert(decay_traits<derived_t>::dimensions() == 3, "Only 2D matrix can be sub square or sub rectangular");
        return etl::dim<1>(as_derived()) == etl::dim<2>(as_derived());
    }

    bool is_sub_rectangular() const noexcept {
        return !is_sub_square();
    }
};

} //end of namespace etl

#endif
