//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains runtime-sized custom matrix implementation
 */

#pragma once

#include "etl/dyn_base.hpp"    //The base class and utilities

namespace etl {

/*!
 * \brief Matrix with run-time fixed dimensions.
 *
 * The matrix support an arbitrary number of dimensions.
 */
template <typename T, order SO, size_t D>
struct custom_dyn_matrix_impl final : dense_dyn_base<custom_dyn_matrix_impl<T, SO, D>, T, SO, D>,
                                      inplace_assignable<custom_dyn_matrix_impl<T, SO, D>>,
                                      expression_able<custom_dyn_matrix_impl<T, SO, D>>,
                                      value_testable<custom_dyn_matrix_impl<T, SO, D>>,
                                      iterable<custom_dyn_matrix_impl<T, SO, D>, SO == order::RowMajor>,
                                      dim_testable<custom_dyn_matrix_impl<T, SO, D>> {
    static constexpr size_t n_dimensions = D;  ///< The number of dimensions
    static constexpr order storage_order      = SO; ///< The storage order
    static constexpr size_t alignment    = 1;  ///< The memory alignment

    using this_type              = custom_dyn_matrix_impl<T, SO, D>;                           ///< The type of this expression
    using iterable_base_type     = iterable<this_type, SO == order::RowMajor>;                 ///< The iterable base type
    using base_type              = dense_dyn_base<custom_dyn_matrix_impl<T, SO, D>, T, SO, D>; ///< The base type
    using value_type             = T;                                                          ///< The value type
    using dimension_storage_impl = std::array<size_t, n_dimensions>;                      ///< The type used to store the dimensions
    using memory_type            = value_type*;                                                ///< The memory type
    using const_memory_type      = const value_type*;                                          ///< The const memory type

    using iterator       = std::conditional_t<SO == order::RowMajor, value_type*, etl::iterator<this_type>>;             ///< The iterator type
    using const_iterator = std::conditional_t<SO == order::RowMajor, const value_type*, etl::iterator<const this_type>>; ///< The const iterator type

    /*!
     * \brief The vectorization type for V
     */
    template<typename V = default_vec>
    using vec_type               = typename V::template vec_type<T>;

private:
    using base_type::_size;
    using base_type::_dimensions;
    using base_type::_memory;

    using base_type::release;
    using base_type::allocate;
    using base_type::check_invariants;

public:
    using base_type::dim;
    using base_type::memory_start;
    using base_type::memory_end;
    using iterable_base_type::begin;
    using iterable_base_type::end;

    // Construction

    /*!
     * \brief Copy construct a custom_dyn_matrix_impl
     * \param rhs The matrix to copy from
     */
    custom_dyn_matrix_impl(const custom_dyn_matrix_impl& rhs) noexcept : base_type(rhs) {
        _memory = rhs._memory;
    }

    /*!
     * \brief Move construct a matrix
     * \param rhs The matrix to move
     */
    custom_dyn_matrix_impl(custom_dyn_matrix_impl&& rhs) noexcept : base_type(std::move(rhs)) {
        _memory = rhs._memory;
        rhs._memory = nullptr;
    }

    /*!
     * \brief Construct a matrix over existing memory
     * \param memory Pointer to the memory
     * \param sizes The dimensions of the matrix
     *
     * The number of dimesnions must be the same as the D template
     * parameter of the matrix.
     *
     * The memory won't be managed, meaning that it won't be
     * released once the matrix is destructed.
     */
    template <typename... S, cpp_enable_if((sizeof...(S) == D))>
    explicit custom_dyn_matrix_impl(value_type* memory, S... sizes) noexcept : base_type(util::size(sizes...), {{static_cast<size_t>(sizes)...}})
                                                    {
        _memory = memory;
        //Nothing else to init
    }

    custom_dyn_matrix_impl& operator=(const custom_dyn_matrix_impl& rhs) noexcept {
        if (this != &rhs) {
            _size       = rhs._size;
            _dimensions = rhs._dimensions;
            _memory     = rhs._memory;
        }

        check_invariants();

        return *this;
    }

    /*!
     * \brief Move assign from another matrix
     *
     * The other matrix won't be usable after the move operation
     *
     * \param rhs The matrix to move from
     * \return A reference to the matrix
     */
    custom_dyn_matrix_impl& operator=(custom_dyn_matrix_impl&& rhs) noexcept {
        if (this != &rhs) {
            _size               = rhs._size;
            _dimensions         = std::move(rhs._dimensions);
            _memory             = rhs._memory;

            rhs._size   = 0;
            rhs._memory = nullptr;
        }

        check_invariants();

        return *this;
    }

    /*!
     * \brief Assign from an ETL expression.
     * \param e The expression containing the values to assign to the matrix
     * \return A reference to the matrix
     */
    template <typename E, cpp_enable_if(!std::is_same<std::decay_t<E>, custom_dyn_matrix_impl<T, SO, D>>::value, std::is_convertible<value_t<E>, value_type>::value, is_etl_expr<E>::value)>
    custom_dyn_matrix_impl& operator=(E&& e) noexcept {
        validate_assign(*this, e);

        e.assign_to(*this);

        check_invariants();

        return *this;
    }

    /*!
     * \brief Assign from an STL container.
     * \param vec The container containing the values to assign to the matrix
     * \return A reference to the matrix
     */
    template <typename Container, cpp_enable_if(!is_etl_expr<Container>::value, std::is_convertible<typename Container::value_type, value_type>::value)>
    custom_dyn_matrix_impl& operator=(const Container& vec) {
        validate_assign(*this, vec);

        std::copy(vec.begin(), vec.end(), begin());

        check_invariants();

        return *this;
    }

    /*!
     * \brief Assign the same value to each element of the matrix
     * \param value The value to assign to each element of the matrix
     * \return A reference to the matrix
     */
    custom_dyn_matrix_impl& operator=(const value_type& value) noexcept {
        std::fill(begin(), end(), value);

        check_invariants();

        return *this;
    }

    /*!
     * \brief Destruct the matrix
     */
    ~custom_dyn_matrix_impl() noexcept {
        // Nothing to do
    }

    /*!
     * \brief Swap the content of the matrix with the content of the given matrix
     * \param other The other matrix to swap content with
     */
    void swap(custom_dyn_matrix_impl& other) {
        using std::swap;
        swap(_size, other._size);
        swap(_dimensions, other._dimensions);
        swap(_memory, other._memory);

        //TODO swap is likely screwing up GPU memory!

        check_invariants();
    }

    // Accessors

    /*!
     * \brief Store several elements in the matrix at once
     * \param in The several elements to store
     * \param i The position at which to start. This will be aligned from the beginning (multiple of the vector size).
     * \tparam V The vectorization mode to use
     */
    template <typename V = default_vec>
    void store(vec_type<V> in, size_t i) noexcept {
        V::storeu(memory_start() + i, in);
    }

    /*!
     * \brief Store several elements in the matrix at once
     * \param in The several elements to store
     * \param i The position at which to start. This will be aligned from the beginning (multiple of the vector size).
     * \tparam V The vectorization mode to use
     */
    template <typename V = default_vec>
    void storeu(vec_type<V> in, size_t i) noexcept {
        V::storeu(memory_start() + i, in);
    }

    /*!
     * \brief Store several elements in the matrix at once, using non-temporal store
     * \param in The several elements to store
     * \param i The position at which to start. This will be aligned from the beginning (multiple of the vector size).
     * \tparam V The vectorization mode to use
     */
    template <typename V = default_vec>
    void stream(vec_type<V> in, size_t i) noexcept {
        V::storeu(memory_start() + i, in);
    }

    /*!
     * \brief Load several elements of the matrix at once
     * \param i The position at which to start. This will be aligned from the beginning (multiple of the vector size).
     * \tparam V The vectorization mode to use
     * \return a vector containing several elements of the matrix
     */
    template<typename V = default_vec>
    vec_type<V> load(size_t i) const noexcept {
        return V::loadu(_memory + i);
    }

    /*!
     * \brief Load several elements of the matrix at once
     * \param i The position at which to start. This will be aligned from the beginning (multiple of the vector size).
     * \tparam V The vectorization mode to use
     * \return a vector containing several elements of the matrix
     */
    template<typename V = default_vec>
    vec_type<V> loadu(size_t i) const noexcept {
        return V::loadu(_memory + i);
    }

    // Assignment functions

    /*!
     * \brief Assign to the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_to(L&& lhs)  const {
        std_assign_evaluate(*this, lhs);
    }

    /*!
     * \brief Add to the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L, cpp_enable_if(std::is_same<value_t<L>, value_type>::value && is_dma<L>::value)>
    void assign_add_to(L&& lhs)  const {
        if(!detail::direct_add(lhs, *this)){
            std_add_evaluate(*this, lhs);
        }
    }

    /*!
     * \brief Add to the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L, cpp_disable_if(std::is_same<value_t<L>, value_type>::value && is_dma<L>::value)>
    void assign_add_to(L&& lhs)  const {
        std_add_evaluate(*this, lhs);
    }

    /*!
     * \brief Subtract from the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L, cpp_enable_if(std::is_same<value_t<L>, value_type>::value && is_dma<L>::value)>
    void assign_sub_to(L&& lhs)  const {
        if(!detail::direct_sub(lhs, *this)){
            std_sub_evaluate(*this, lhs);
        }
    }

    /*!
     * \brief Subtract from the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L, cpp_disable_if(std::is_same<value_t<L>, value_type>::value && is_dma<L>::value)>
    void assign_sub_to(L&& lhs)  const {
        std_sub_evaluate(*this, lhs);
    }

    /*!
     * \brief Multiply the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L, cpp_enable_if(std::is_same<value_t<L>, value_type>::value && is_dma<L>::value)>
    void assign_mul_to(L&& lhs)  const {
        if(!detail::direct_mul(lhs, *this)){
            std_mul_evaluate(*this, lhs);
        }
    }

    /*!
     * \brief Multiply the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L, cpp_disable_if(std::is_same<value_t<L>, value_type>::value && is_dma<L>::value)>
    void assign_mul_to(L&& lhs)  const {
        std_mul_evaluate(*this, lhs);
    }
    /*!
     * \brief Divide the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L, cpp_enable_if(std::is_same<value_t<L>, value_type>::value && is_dma<L>::value)>
    void assign_div_to(L&& lhs)  const {
        if(!detail::direct_div(lhs, *this)){
            std_div_evaluate(*this, lhs);
        }
    }

    /*!
     * \brief Divide to the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L, cpp_disable_if(std::is_same<value_t<L>, value_type>::value && is_dma<L>::value)>
    void assign_div_to(L&& lhs)  const {
        std_div_evaluate(*this, lhs);
    }

    /*!
     * \brief Modulo the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_mod_to(L&& lhs)  const {
        std_mod_evaluate(*this, lhs);
    }

    // Internals

    /*!
     * \brief Apply the given visitor to this expression and its descendants.
     * \param visitor The visitor to apply
     */
    void visit(const detail::temporary_allocator_visitor& visitor) const {
        cpp_unused(visitor);
    }

    /*!
     * \brief Apply the given visitor to this expression and its descendants.
     * \param visitor The visitor to apply
     */
    void visit(const detail::back_propagate_visitor& visitor) const {
        cpp_unused(visitor);
    }

    /*!
     * \brief Apply the given visitor to this expression and its descendants.
     * \param visitor The visitor to apply
     */
    void visit(const detail::evaluator_visitor& visitor) const {
        cpp_unused(visitor);
    }

    /*!
     * \brief Print the description of the matrix to the given stream
     * \param os The output stream
     * \param mat The matrix to output the description to the stream
     * \return The given output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const custom_dyn_matrix_impl& mat) {
        if (D == 1) {
            return os << "CV[" << mat.size() << "]";
        }

        os << "CM[" << mat.dim(0);

        for (size_t i = 1; i < D; ++i) {
            os << "," << mat.dim(i);
        }

        return os << "]";
    }
};

static_assert(std::is_nothrow_move_constructible<dyn_vector<double>>::value, "dyn_vector should be nothrow move constructible");
static_assert(std::is_nothrow_move_assignable<dyn_vector<double>>::value, "dyn_vector should be nothrow move assignable");
static_assert(std::is_nothrow_destructible<dyn_vector<double>>::value, "dyn_vector should be nothrow destructible");

/*!
 * \brief Swap two dyn matrix
 * \param lhs The first matrix
 * \param rhs The second matrix
 */
template <typename T, order SO, size_t D>
void swap(custom_dyn_matrix_impl<T, SO, D>& lhs, custom_dyn_matrix_impl<T, SO, D>& rhs) {
    lhs.swap(rhs);
}

} //end of namespace etl
