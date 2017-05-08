//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief memory_slice_view expression implementation
 */

#pragma once

namespace etl {

/*!
 * \brief View that shows a slice of an expression
 * \tparam T The type of expression on which the view is made
 */
template <typename T, bool Aligned>
struct memory_slice_view {
    using sub_type          = T;                                                                    ///< The sub type
    using value_type        = value_t<sub_type>;                                                    ///< The value contained in the expression
    using memory_type       = value_type*;                                                          ///< The memory acess type
    using const_memory_type = const value_type*;                                                    ///< The const memory access type
    using return_type       = return_helper<sub_type, decltype(std::declval<sub_type>()[0])>;       ///< The type returned by the view
    using const_return_type = const_return_helper<sub_type, decltype(std::declval<sub_type>()[0])>; ///< The const type return by the view

private:
    T sub;              ///< The Sub expression
    const size_t first; ///< The index
    const size_t last;  ///< The last index

    friend struct etl_traits<memory_slice_view>;

public:

    /*!
     * \brief The vectorization type for V
     */
    template<typename V = default_vec>
    using vec_type               = typename V::template vec_type<value_type>;

    /*!
     * \brief Construct a new memory_slice_view over the given sub expression
     * \param sub The sub expression
     * \param first The first index
     * \param last The last index
     */
    memory_slice_view(sub_type sub, size_t first, size_t last)
            : sub(sub), first(first), last(last) {}

    /*!
     * \brief Returns the element at the given index
     * \param j The index
     * \return a reference to the element at the given index.
     */
    const_return_type operator[](size_t j) const {
        return sub[first + j];
    }

    /*!
     * \brief Returns the element at the given index
     * \param j The index
     * \return a reference to the element at the given index.
     */
    return_type operator[](size_t j) {
        return sub[first + j];
    }

    /*!
     * \brief Returns the value at the given index
     * This function never has side effects.
     * \param j The index
     * \return the value at the given index.
     */
    value_type read_flat(size_t j) const noexcept {
        return sub[first + j];
    }

    //Note(CPP17): Use if constexpr for alignment

    /*!
     * \brief Load several elements of the expression at once
     * \param x The position at which to start.
     * \tparam V The vectorization mode to use
     * \return a vector containing several elements of the expression
     */
    template <typename V = default_vec, bool A = Aligned, cpp_enable_if(A)>
    auto load(size_t x) const noexcept {
        return sub.template load<V>(x + first );
    }

    /*!
     * \brief Load several elements of the expression at once
     * \param x The position at which to start.
     * \tparam V The vectorization mode to use
     * \return a vector containing several elements of the expression
     */
    template <typename V = default_vec, bool A = Aligned, cpp_disable_if(A)>
    auto load(size_t x) const noexcept {
        return sub.template loadu<V>(x + first );
    }

    /*!
     * \brief Load several elements of the expression at once
     * \param x The position at which to start.
     * \tparam V The vectorization mode to use
     * \return a vector containing several elements of the expression
     */
    template <typename V = default_vec>
    auto loadu(size_t x) const noexcept {
        return sub.template loadu<V>(x + first );
    }

    /*!
     * \brief Store several elements in the matrix at once
     * \param in The several elements to store
     * \param i The position at which to start. This will be aligned from the beginning (multiple of the vector size).
     * \tparam V The vectorization mode to use
     */
    template <typename V = default_vec, bool A = Aligned, cpp_enable_if(A)>
    void store(vec_type<V> in, size_t i) noexcept {
        sub.template store<V>(in, first + i);
    }

    /*!
     * \brief Store several elements in the matrix at once
     * \param in The several elements to store
     * \param i The position at which to start. This will be aligned from the beginning (multiple of the vector size).
     * \tparam V The vectorization mode to use
     */
    template <typename V = default_vec, bool A = Aligned, cpp_disable_if(A)>
    void store(vec_type<V> in, size_t i) noexcept {
        sub.template storeu<V>(in, first + i);
    }

    /*!
     * \brief Store several elements in the matrix at once
     * \param in The several elements to store
     * \param i The position at which to start. This will be aligned from the beginning (multiple of the vector size).
     * \tparam V The vectorization mode to use
     */
    template <typename V = default_vec>
    void storeu(vec_type<V> in, size_t i) noexcept {
        sub.template storeu<V>(in, first + i);
    }

    /*!
     * \brief Store several elements in the matrix at once, using non-temporal store
     * \param in The several elements to store
     * \param i The position at which to start. This will be aligned from the beginning (multiple of the vector size).
     * \tparam V The vectorization mode to use
     */
    template <typename V = default_vec>
    void stream(vec_type<V> in, size_t i) noexcept {
        sub.template storeu<V>(in, first + i);
    }

    /*!
     * \brief Test if this expression aliases with the given expression
     * \param rhs The other expression to test
     * \return true if the two expressions aliases, false otherwise
     */
    template <typename E>
    bool alias(const E& rhs) const noexcept {
        return sub.alias(rhs);
    }

    /*!
     * \brief Returns a pointer to the first element in memory.
     * \return a pointer tot the first element in memory.
     */
    memory_type memory_start() noexcept {
        return sub.memory_start() + first ;
    }

    /*!
     * \brief Returns a pointer to the first element in memory.
     * \return a pointer tot the first element in memory.
     */
    const_memory_type memory_start() const noexcept {
        return sub.memory_start() + first ;
    }

    /*!
     * \brief Returns a pointer to the past-the-end element in memory.
     * \return a pointer tot the past-the-end element in memory.
     */
    memory_type memory_end() noexcept {
        return sub.memory_start() + last ;
    }

    /*!
     * \brief Returns a pointer to the past-the-end element in memory.
     * \return a pointer tot the past-the-end element in memory.
     */
    const_memory_type memory_end() const noexcept {
        return sub.memory_start() + last;
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
    template<typename L>
    void assign_add_to(L&& lhs)  const {
        std_add_evaluate(*this, lhs);
    }

    /*!
     * \brief Sub from the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_sub_to(L&& lhs)  const {
        std_sub_evaluate(*this, lhs);
    }

    /*!
     * \brief Multiply the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_mul_to(L&& lhs)  const {
        std_mul_evaluate(*this, lhs);
    }

    /*!
     * \brief Divide the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
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
    void visit(const detail::back_propagate_visitor& visitor) const {
        sub.visit(visitor);
    }

    /*!
     * \brief Apply the given visitor to this expression and its descendants.
     * \param visitor The visitor to apply
     */
    void visit(const detail::temporary_allocator_visitor& visitor) const {
        sub.visit(visitor);
    }

    /*!
     * \brief Apply the given visitor to this expression and its descendants.
     * \param visitor The visitor to apply
     */
    void visit(detail::evaluator_visitor& visitor) const {
        bool old_need_value = visitor.need_value;
        visitor.need_value = true;
        sub.visit(visitor);
        visitor.need_value = old_need_value;
    }
};

/*!
 * \brief Specialization for memory_slice_view
 */
template <typename T, bool Aligned>
struct etl_traits<etl::memory_slice_view<T, Aligned>> {
    using expr_t     = etl::memory_slice_view<T, Aligned>; ///< The expression type
    using sub_expr_t = std::decay_t<T>;    ///< The sub expression type
    using value_type = typename etl_traits<sub_expr_t>::value_type; ///< The value type

    static constexpr bool is_etl          = true;                                    ///< Indicates if the type is an ETL expression
    static constexpr bool is_transformer  = false;                                   ///< Indicates if the type is a transformer
    static constexpr bool is_view         = true;                                    ///< Indicates if the type is a view
    static constexpr bool is_magic_view   = false;                                   ///< Indicates if the type is a magic view
    static constexpr bool is_fast         = false;                                   ///< Indicates if the expression is fast
    static constexpr bool is_linear       = etl_traits<sub_expr_t>::is_linear;       ///< Indicates if the expression is linear
    static constexpr bool is_thread_safe  = etl_traits<sub_expr_t>::is_thread_safe;  ///< Indicates if the expression is thread safe
    static constexpr bool is_value        = false;                                   ///< Indicates if the expression is of value type
    static constexpr bool is_direct       = etl_traits<sub_expr_t>::is_direct;       ///< Indicates if the expression has direct memory access
    static constexpr bool is_generator    = false;                                   ///< Indicates if the expression is a generator
    static constexpr bool is_padded       = false;                                   ///< Indicates if the expression is padded
    static constexpr bool is_aligned      = Aligned;                                   ///< Indicates if the expression is padded
    static constexpr bool needs_evaluator = etl_traits<sub_expr_t>::needs_evaluator; ///< Indicates if the exxpression needs a evaluator visitor
    static constexpr order storage_order  = etl_traits<sub_expr_t>::storage_order;   ///< The expression's storage order

    /*!
     * \brief Indicates if the expression is vectorizable using the
     * given vector mode
     * \tparam V The vector mode
     */
    template <vector_mode_t V>
    using vectorizable = cpp::bool_constant<decay_traits<sub_expr_t>::template vectorizable<V>::value>;

    /*!
     * \brief Returns the size of the given expression
     * \param v The expression to get the size for
     * \returns the size of the given expression
     */
    static size_t size(const expr_t& v) {
        return v.last - v.first;
    }

    /*!
     * \brief Returns the dth dimension of the given expression
     * \param v The expression
     * \param d The dimension to get
     * \return The dth dimension of the given expression
     */
    static size_t dim(const expr_t& v, size_t d) {
        cpp_unused(d);
        return v.last - v.first;
    }

    /*!
     * \brief Returns the number of expressions for this type
     * \return the number of dimensions of this type
     */
    static constexpr size_t dimensions() {
        return 1;
    }
};

} //end of namespace etl
