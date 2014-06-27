//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef ETL_FAST_VECTOR_HPP
#define ETL_FAST_VECTOR_HPP

#include <cstddef>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <array>
#include <vector>
#include <numeric>

#include "assert.hpp"
#include "tmp.hpp"
#include "fast_op.hpp"
#include "fast_expr.hpp"

namespace etl {

template<typename T, std::size_t Rows>
class fast_vector {
private:
    std::array<T, Rows> _data;

public:
    static constexpr const std::size_t rows = Rows;

    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;

    fast_vector(){
        //Nothing else to init
    }

    fast_vector(const T& value){
        std::fill(_data.begin(), _data.end(), value);
    }

    //Copy assignment operator

    fast_vector& operator=(const fast_vector& rhs){
        for(std::size_t i = 0; i < Rows; ++i){
            _data[i] = rhs[i];
        }

        return *this;
    }

    //Construct from expression

    template<typename LE, typename Op, typename RE>
    fast_vector(binary_expr<T, LE, Op, RE>&& e){
        for(std::size_t i = 0; i < Rows; ++i){
            _data[i] = e[i];
        }
    }

    template<typename LE, typename Op, typename RE>
    fast_vector& operator=(binary_expr<T, LE, Op, RE>&& e){
        for(std::size_t i = 0; i < Rows; ++i){
            _data[i] = e[i];
        }

        return *this;
    }

    //Allow copy from other containers

    template<typename Container, enable_if_u<std::is_same<Container::value_type, T>::value> = detail::dummy>
    fast_vector& operator=(const Container& vec){
        etl_assert(vec.size() == Rows, "Cannot copy from a vector of different size");

        for(std::size_t i = 0; i < Rows; ++i){
            _data[i] = vec[i];
        }

        return *this;
    }

    //Prohibit copy
    fast_vector(const fast_vector& rhs) = delete;

    //Allow move
    fast_vector(fast_vector&& rhs) = default;
    fast_vector& operator=(fast_vector&& rhs) = default;

    //Modifiers

    //Set every element to the same scalar
    void operator=(const T& value){
        std::fill(_data.begin(), _data.end(), value);
    }

    //Multiply each element by a scalar
    fast_vector& operator*=(const T& value){
        for(size_t i = 0; i < Rows; ++i){
            _data[i] *= value;
        }

        return *this;
    }

    //Divide each element by a scalar
    fast_vector& operator/=(const T& value){
        for(size_t i = 0; i < Rows; ++i){
            _data[i] /= value;
        }

        return *this;
    }

    template<typename RE>
    fast_vector& operator+=(RE&& rhs){
        for(size_t i = 0; i < Rows; ++i){
            _data[i] += rhs[i];
        }

        return *this;
    }

    template<typename RE>
    fast_vector& operator-=(RE&& rhs){
        for(size_t i = 0; i < Rows; ++i){
            _data[i] -= rhs[i];
        }

        return *this;
    }

    //Add a scalar to each element
    template<typename RE, enable_if_u<std::is_convertible<RE, T>::value> = detail::dummy>
    auto operator+(RE re) const -> binary_expr<T, const fast_vector&, plus_binary_op<T>, scalar<T>> {
        return {*this, re};
    }

    //Add elements of vector together
    template<typename RE, disable_if_u<std::is_convertible<RE, T>::value> = detail::dummy>
    auto operator+(RE&& re) const -> binary_expr<T, const fast_vector&, plus_binary_op<T>, decltype(std::forward<RE>(re))> {
        return {*this, std::forward<RE>(re)};
    }

    //Remove each element by a scalar
    template<typename RE, enable_if_u<std::is_convertible<RE, T>::value> = detail::dummy>
    auto operator-(RE re) const -> binary_expr<T, const fast_vector&, minus_binary_op<T>, scalar<T>> {
        return {*this, re};
    }

    //Sub elements of vector together
    template<typename RE, disable_if_u<std::is_convertible<RE, T>::value> = detail::dummy>
    auto operator-(RE&& re) const -> binary_expr<T, const fast_vector&, minus_binary_op<T>, decltype(std::forward<RE>(re))> {
        return {*this, std::forward<RE>(re)};
    }

    //Mul elements of vector togethers
    template<typename RE, disable_if_u<std::is_convertible<RE, T>::value> = detail::dummy>
    auto operator*(RE&& re) const -> binary_expr<T, const fast_vector&, mul_binary_op<T>, decltype(std::forward<RE>(re))> {
        return {*this, std::forward<RE>(re)};
    }

    //Div elements of vector togethers
    template<typename RE, disable_if_u<std::is_convertible<RE, T>::value> = detail::dummy>
    auto operator/(RE&& re) const -> binary_expr<T, const fast_vector&, div_binary_op<T>, decltype(std::forward<RE>(re))> {
        return {*this, std::forward<RE>(re)};
    }

    //Accessors

    constexpr size_t size() const {
        return rows;
    }

    T& operator()(size_t i){
        etl_assert(i < rows, "Out of bounds");

        return _data[i];
    }

    const T& operator()(size_t i) const {
        etl_assert(i < rows, "Out of bounds");

        return _data[i];
    }

    T& operator[](size_t i){
        etl_assert(i < rows, "Out of bounds");

        return _data[i];
    }

    const T& operator[](size_t i) const {
        etl_assert(i < rows, "Out of bounds");

        return _data[i];
    }

    const_iterator begin() const {
        return _data.begin();
    }

    iterator begin(){
        return _data.begin();
    }

    const_iterator end() const {
        return _data.end();
    }

    iterator end(){
        return _data.end();
    }
};

template<typename T, std::size_t Rows, typename RE, enable_if_u<std::is_convertible<RE, T>::value> = detail::dummy>
auto operator*(const fast_vector<T, Rows>& lhs, RE rhs) -> binary_expr<T, const fast_vector<T, Rows>&, mul_binary_op<T>, scalar<T>> {
    return {lhs, rhs};
}

template<typename T, std::size_t Rows, typename RE, enable_if_u<std::is_convertible<RE, T>::value> = detail::dummy>
auto operator*(RE lhs, const fast_vector<T, Rows>& rhs) -> binary_expr<T, scalar<T>, mul_binary_op<T>, const fast_vector<T, Rows>&> {
    return {lhs, rhs};
}

template<typename T, std::size_t Rows, typename RE, enable_if_u<std::is_convertible<RE, T>::value> = detail::dummy>
auto operator/(const fast_vector<T, Rows>& lhs, RE rhs) -> binary_expr<T, const fast_vector<T, Rows>&, div_binary_op<T>, scalar<T>> {
    return {lhs, rhs};
}

template<typename T, std::size_t Rows>
auto abs(const fast_vector<T, Rows>& value) -> unary_expr<T, const fast_vector<T, Rows>&, abs_unary_op<T>> {
    return {value};
}

template<typename T, std::size_t Rows>
auto sign(const fast_vector<T, Rows>& value) -> unary_expr<T, const fast_vector<T, Rows>&, sign_unary_op<T>> {
    return {value};
}

template<typename T, std::size_t Rows>
T sum(const fast_vector<T, Rows>& values){
    return std::accumulate(values.begin(), values.end(), static_cast<T>(0));
}

} //end of namespace etl

#endif
