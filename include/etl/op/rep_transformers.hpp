//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

namespace etl {

/*!
 * \brief Abstract repeat Transformer that repeats the expression to the right
 * \tparam T The type on which the transformer is applied
 */
template <typename T, typename D>
struct rep_transformer {
    using derived_t  = D;          ///< The derived type
    using sub_type   = T;          ///< The type on which the expression works
    using value_type = value_t<T>; ///< The type of valuie

protected:
    sub_type sub; ///< The subexpression

public:
    /*!
     * \brief Construct a new transformer around the given expression
     * \param expr The sub expression
     */
    explicit rep_transformer(sub_type expr)
            : sub(expr) {}

    /*!
     * \brief Access to the value at the given (args...) position
     * \param args The indices
     * \return The value at the position (args...)
     */
    template <typename... Sizes>
    value_type operator()(Sizes... args) const {
        return as_derived().selected_only(make_index_range<derived_t::dim_start, derived_t::dim_end>(), args...);
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

    // Internals

    /*!
     * \brief Apply the given visitor to this expression and its descendants.
     * \param visitor The visitor to apply
     */
    template<typename V>
    void visit(V&& visitor) const {
        sub.visit(std::forward<V>(visitor));
    }

private:
    /*!
     * \brief Returns a const reference to the derived object, i.e. the object using the CRTP injector.
     * \return a const reference to the derived object.
     */
    const derived_t& as_derived() const noexcept {
        return *static_cast<const derived_t*>(this);
    }
};

/*!
 * \brief Transform that repeats the expression to the right
 * \tparam T The type on which the transformer is applied
 * \tparam D The new dimensions
 */
template <typename T, size_t... D>
struct rep_r_transformer : rep_transformer<T, rep_r_transformer<T,D...>> {
    using this_type  = rep_r_transformer<T, D...>;     ///< This type
    using base_type  = rep_transformer<T, this_type>;  ///< The base type
    using sub_type   = typename base_type::sub_type;   ///< The type on which the expression works
    using value_type = typename base_type::value_type; ///< The type of value

private:

    static constexpr size_t sub_d      = decay_traits<sub_type>::dimensions(); ///< The number of dimensions of the sub type
    static constexpr size_t dimensions = sizeof...(D) + sub_d;                 ///< The number of dimensions of the transformer
    static constexpr size_t dim_start  = 0;                                    ///< First dimension to take into account
    static constexpr size_t dim_end    = sub_d;                                ///< Last dimension to take into account

    friend struct rep_transformer<T, rep_r_transformer>;
    friend struct etl_traits<rep_r_transformer>;

public:

    /*!
     * \brief Construct a new transformer around the given expression
     * \param expr The sub expression
     */
    explicit rep_r_transformer(sub_type expr)
            : base_type(expr) {}

    /*!
     * \brief Returns the value at the given index
     * \param i The index
     * \return the value at the given index.
     */
    value_type operator[](size_t i) const {
        return this->sub[i / mul_all<D...>::value];
    }

    /*!
     * \brief Returns the value at the given index
     * This function never has side effects.
     * \param i The index
     * \return the value at the given index.
     */
    value_type read_flat(size_t i) const noexcept {
        return this->sub.read_flat(i / mul_all<D...>::value);
    }

    /*!
     * \brief Returns the value at the given indices inside the range
     */
    template <typename... Sizes, size_t... I>
    value_type selected_only(const std::index_sequence<I...>& /*seq*/, Sizes... sizes) const {
        return this->sub(cpp::nth_value<I>(sizes...)...);
    }

    /*!
     * \brief Display the transformer on the given stream
     * \param os The output stream
     * \param transformer The transformer to print
     * \return the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const rep_r_transformer& transformer) {
        return os << "rep_r[" << concat_sizes(D...) << "](" << transformer.sub << ")";
    }
};

/*!
 * \brief Transform that repeats the expression to the left
 * \tparam T The type on which the transformer is applied
 * \tparam D The new dimensions
 */
template <typename T, size_t... D>
struct rep_l_transformer : rep_transformer<T, rep_l_transformer<T,D...>> {
    using this_type  = rep_l_transformer<T, D...>;     ///< This type
    using base_type  = rep_transformer<T, this_type>;  ///< The base type
    using sub_type   = typename base_type::sub_type;   ///< The type on which the expression works
    using value_type = typename base_type::value_type; ///< The type of value

private:

    static constexpr size_t sub_d      = decay_traits<sub_type>::dimensions(); ///< The number of dimensions of the sub type
    static constexpr size_t dimensions = sizeof...(D) + sub_d;                 ///< The number of dimensions of the transformer
    static constexpr size_t dim_start  = sizeof...(D);                         ///< Last dimension to take into account
    static constexpr size_t dim_end    = dimensions;                           ///< Last dimension to take into account

    friend struct rep_transformer<T, rep_l_transformer>;
    friend struct etl_traits<rep_l_transformer>;

public:

    /*!
     * \brief Construct a new transformer around the given expression
     * \param expr The sub expression
     */
    explicit rep_l_transformer(sub_type expr)
            : base_type(expr) {}

    /*!
     * \brief Returns the value at the given index
     * \param i The index
     * \return the value at the given index.
     */
    value_type operator[](size_t i) const {
        return this->sub[i % size(this->sub)];
    }

    /*!
     * \brief Returns the value at the given index
     * This function never has side effects.
     * \param i The index
     * \return the value at the given index.
     */
    value_type read_flat(size_t i) const noexcept {
        return this->sub.read_flat(i % size(this->sub));
    }

    /*!
     * \brief Returns the value at the given indices inside the range
     */
    template <typename... Sizes, size_t... I>
    value_type selected_only(const std::index_sequence<I...>& /*seq*/, Sizes... sizes) const {
        return this->sub(cpp::nth_value<I>(sizes...)...);
    }

    /*!
     * \brief Display the transformer on the given stream
     * \param os The output stream
     * \param transformer The transformer to print
     * \return the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const rep_l_transformer& transformer) {
        return os << "rep_l[" << concat_sizes(D...) << "](" << transformer.sub << ")";
    }
};

/*!
 * \brief Transform (dynamic) that repeats the expression to the right
 * \tparam T The type on which the transformer is applied
 * \tparam D The number of new dimensions
 */
template <typename T, size_t D>
struct dyn_rep_r_transformer : rep_transformer<T, dyn_rep_r_transformer<T, D>> {
    using this_type  = dyn_rep_r_transformer<T, D>;    ///< This type
    using base_type  = rep_transformer<T, this_type>;  ///< The base type
    using sub_type   = typename base_type::sub_type;   ///< The type on which the expression works
    using value_type = typename base_type::value_type; ///< The type of value

private:

    static constexpr size_t sub_d      = decay_traits<sub_type>::dimensions(); ///< The number of dimensions of the sub type
    static constexpr size_t dimensions = D + sub_d;                            ///< The number of dimensions of the transformer
    static constexpr size_t dim_start  = 0;                                    ///< First dimension to take into account
    static constexpr size_t dim_end    = sub_d;                                ///< Last dimension to take into account

    std::array<size_t, D> reps; ///< The repeated dimensions
    size_t m;                   ///< The repeated size

    friend struct rep_transformer<T, dyn_rep_r_transformer>;
    friend struct etl_traits<dyn_rep_r_transformer>;

public:

    /*!
     * \brief Construct a new transformer around the given expression
     * \param expr The sub expression
     * \param reps_a The repeated dimensions
     */
    dyn_rep_r_transformer(sub_type expr, std::array<size_t, D> reps_a)
            : base_type(expr), reps(reps_a) {
        m = std::accumulate(reps.begin(), reps.end(), 1UL,
                            [](size_t a, size_t b) {
                                return a * b;
                            });
    }

    /*!
     * \brief Returns the value at the given index
     * \param i The index
     * \return the value at the given index.
     */
    value_type operator[](size_t i) const {
        return this->sub[i / m];
    }

    /*!
     * \brief Returns the value at the given index
     * This function never has side effects.
     * \param i The index
     * \return the value at the given index.
     */
    value_type read_flat(size_t i) const noexcept {
        return this->sub.read_flat(i / m);
    }

    /*!
     * \brief Returns the value at the given indices inside the range
     */
    template <typename... Sizes, size_t... I>
    value_type selected_only(const std::index_sequence<I...>& /*seq*/, Sizes... sizes) const {
        return this->sub(cpp::nth_value<I>(sizes...)...);
    }
};

/*!
 * \brief Transform (dynamic) that repeats the expression to the left
 * \tparam T The type on which the transformer is applied
 * \tparam D The number of new dimensions
 */
template <typename T, size_t D>
struct dyn_rep_l_transformer : rep_transformer<T, dyn_rep_l_transformer<T, D>> {
    using this_type  = dyn_rep_l_transformer<T, D>;    ///< This type
    using base_type  = rep_transformer<T, this_type>;  ///< The base type
    using sub_type   = typename base_type::sub_type;   ///< The type on which the expression works
    using value_type = typename base_type::value_type; ///< The type of value

private:

    static constexpr size_t sub_d      = decay_traits<sub_type>::dimensions(); ///< The number of dimensions of the sub type
    static constexpr size_t dimensions = D + sub_d;                            ///< The number of dimensions of the transformer
    static constexpr size_t dim_start  = D;                                    ///< First dimension to take into account
    static constexpr size_t dim_end    = dimensions;                           ///< Last dimension to take into account

    std::array<size_t, D> reps; ///< The repeated dimensions
    size_t m;                   ///< The repeated size

    friend struct rep_transformer<T, dyn_rep_l_transformer>;
    friend struct etl_traits<dyn_rep_l_transformer>;

public:

    /*!
     * \brief Construct a new transformer around the given expression
     * \param expr The sub expression
     * \param reps_a The repeated dimensions
     */
    dyn_rep_l_transformer(sub_type expr, std::array<size_t, D> reps_a)
            : base_type(expr), reps(reps_a) {
        m = std::accumulate(reps.begin(), reps.end(), 1UL,
                            [](size_t a, size_t b) {
                                return a * b;
                            });
    }

    /*!
     * \brief Returns the value at the given index
     * \param i The index
     * \return the value at the given index.
     */
    value_type operator[](size_t i) const {
        return this->sub[i % size(this->sub)];
    }

    /*!
     * \brief Returns the value at the given index
     * This function never has side effects.
     * \param i The index
     * \return the value at the given index.
     */
    value_type read_flat(size_t i) const {
        return this->sub.read_flat(i % size(this->sub));
    }

    /*!
     * \brief Returns the value at the given indices inside the range
     */
    template <typename... Sizes, size_t... I>
    value_type selected_only(const std::index_sequence<I...>& /*seq*/, Sizes... sizes) const {
        return this->sub(cpp::nth_value<I>(sizes...)...);
    }
};

/*!
 * \brief Specialization for rep_r_transformer
 */
template <typename T, size_t... D>
struct etl_traits<rep_r_transformer<T, D...>> {
    using expr_t     = etl::rep_r_transformer<T, D...>;             ///< The expression type
    using sub_expr_t = std::decay_t<T>;                             ///< The sub expression type
    using value_type = typename etl_traits<sub_expr_t>::value_type; ///< The value type of this expression

    static constexpr bool is_etl                  = true;                                            ///< Indicates if the type is an ETL expression
    static constexpr bool is_transformer          = true;                                            ///< Indicates if the type is a transformer
    static constexpr bool is_view                 = false;                                           ///< Indicates if the type is a view
    static constexpr bool is_magic_view           = false;                                           ///< Indicates if the type is a magic view
    static constexpr bool is_fast                 = etl_traits<sub_expr_t>::is_fast;                 ///< Indicates if the expression is fast
    static constexpr bool is_linear               = false;                                           ///< Indicates if the expression is linear
    static constexpr bool is_thread_safe          = etl_traits<sub_expr_t>::is_thread_safe;          ///< Indicates if the expression is thread safe
    static constexpr bool is_value                = false;                                           ///< Indicates if the expression is of value type
    static constexpr bool is_direct               = false;           ///< Indicates if the expression has direct memory access
    static constexpr bool is_generator            = false;                                           ///< Indicates if the expression is a generated
    static constexpr bool is_padded               = false;                          ///< Indicates if the expression is padded
    static constexpr bool is_aligned               = false;                          ///< Indicates if the expression is padded
    static constexpr bool is_temporary = etl_traits<sub_expr_t>::is_temporary; ///< Indicaes if the expression needs an evaluator visitor
    static constexpr order storage_order          = etl_traits<sub_expr_t>::storage_order;           ///< The expression storage order

    static constexpr size_t sub_d = etl_traits<sub_expr_t>::dimensions(); ///< The number of dimensions of the sub type

    /*!
     * \brief Indicates if the expression is vectorizable using the
     * given vector mode
     * \tparam V The vector mode
     */
    template <vector_mode_t V>
    using vectorizable = std::false_type;

    /*!
     * \brief Returns the size of the given expression
     * \param v The expression to get the size for
     * \returns the size of the given expression
     */
    static size_t size(const expr_t& v) {
        return mul_all<D...>::value * etl_traits<sub_expr_t>::size(v.sub);
    }

    /*!
     * \brief Returns the dth dimension of the given expression
     * \param v The expression
     * \param d The dimension to get
     * \return The dth dimension of the given expression
     */
    static size_t dim(const expr_t& v, size_t d) {
        if(d < sub_d){
            return etl_traits<sub_expr_t>::dim(v.sub, d);
        } else {
            return dyn_nth_size<D...>(d - sub_d);
        }
    }

    /*!
     * \brief Returns the size of an expression of this fast type.
     * \returns the size of an expression of this fast type.
     */
    static constexpr size_t size() {
        return mul_all<D...>::value * etl_traits<sub_expr_t>::size();
    }

    /*!
     * \brief Returns the D2th dimension of the expression
     */
    template <size_t D2, cpp_enable_if(D2 < sub_d)>
    static constexpr size_t dim() {
        return etl_traits<sub_expr_t>::template dim<D2>();
    }

    /*!
     * \brief Returns the D2th dimension of the expression
     */
    template <size_t D2, cpp_disable_if(D2 < sub_d)>
    static constexpr size_t dim() {
        return nth_size<D2 - sub_d, 0, D...>::value;
    }

    /*!
     * \brief Returns the number of expressions for this type
     * \return the number of dimensions of this type
     */
    static constexpr size_t dimensions() {
        return sizeof...(D) + etl_traits<sub_expr_t>::dimensions();
    }
};

/*!
 * \brief Specialization for rep_l_transformer
 */
template <typename T, size_t... D>
struct etl_traits<rep_l_transformer<T, D...>> {
    using expr_t     = etl::rep_l_transformer<T, D...>;             ///< The expression type
    using sub_expr_t = std::decay_t<T>;                             ///< The sub expression type
    using value_type = typename etl_traits<sub_expr_t>::value_type; ///< The value type of this expression

    static constexpr bool is_etl                  = true;                                            ///< Indicates if the type is an ETL expression
    static constexpr bool is_transformer          = true;                                            ///< Indicates if the type is a transformer
    static constexpr bool is_view                 = false;                                           ///< Indicates if the type is a view
    static constexpr bool is_magic_view           = false;                                           ///< Indicates if the type is a magic view
    static constexpr bool is_fast                 = etl_traits<sub_expr_t>::is_fast;                 ///< Indicates if the expression is fast
    static constexpr bool is_linear               = false;                                           ///< Indicates if the expression is linear
    static constexpr bool is_thread_safe                 = etl_traits<sub_expr_t>::is_thread_safe;                 ///< Indicates if the expression is thread safe
    static constexpr bool is_value                = false;                                           ///< Indicates if the expression is of value type
    static constexpr bool is_direct               = false;           ///< Indicates if the expression has direct memory access
    static constexpr bool is_generator            = false;                                           ///< Indicates if the expression is a generated
    static constexpr bool is_padded               = false;                          ///< Indicates if the expression is padded
    static constexpr bool is_aligned               = false;                          ///< Indicates if the expression is padded
    static constexpr bool is_temporary = etl_traits<sub_expr_t>::is_temporary; ///< Indicaes if the expression needs an evaluator visitor
    static constexpr order storage_order          = etl_traits<sub_expr_t>::storage_order;           ///< The expression storage order

    /*!
     * \brief Indicates if the expression is vectorizable using the
     * given vector mode
     * \tparam V The vector mode
     */
    template <vector_mode_t V>
    using vectorizable = std::false_type;

    /*!
     * \brief Returns the size of the given expression
     * \param v The expression to get the size for
     * \returns the size of the given expression
     */
    static size_t size(const expr_t& v) {
        return mul_all<D...>::value * etl_traits<sub_expr_t>::size(v.sub);
    }

    /*!
     * \brief Returns the dth dimension of the given expression
     * \param v The expression
     * \param d The dimension to get
     * \return The dth dimension of the given expression
     */
    static size_t dim(const expr_t& v, size_t d) {
        if(d >= sizeof...(D)){
            return etl_traits<sub_expr_t>::dim(v.sub, d - sizeof...(D));
        } else {
            return dyn_nth_size<D...>(d);
        }
    }

    /*!
     * \brief Returns the size of an expression of this fast type.
     * \returns the size of an expression of this fast type.
     */
    static constexpr size_t size() {
        return mul_all<D...>::value * etl_traits<sub_expr_t>::size();
    }

    /*!
     * \brief Returns the D2th dimension of an expression of this type
     * \tparam D2 The dimension to get
     * \return the D2th dimension of an expression of this type
     */
    template <size_t D2, cpp_enable_if((D2 >= sizeof...(D)))>
    static constexpr size_t dim() {
        return etl_traits<sub_expr_t>::template dim<D2 - sizeof...(D)>();
    }

    /*!
     * \brief Returns the D2th dimension of an expression of this type
     * \tparam D2 The dimension to get
     * \return the D2th dimension of an expression of this type
     */
    template <size_t D2, cpp_disable_if((D2 >= sizeof...(D)))>
    static constexpr size_t dim() {
        return nth_size<D2, 0, D...>::value;
    }

    /*!
     * \brief Returns the number of expressions for this type
     * \return the number of dimensions of this type
     */
    static constexpr size_t dimensions() {
        return sizeof...(D) + etl_traits<sub_expr_t>::dimensions();
    }
};

/*!
 * \brief Specialization for dyn_rep_r_transformer
 */
template <typename T, size_t D>
struct etl_traits<dyn_rep_r_transformer<T, D>> {
    using expr_t     = etl::dyn_rep_r_transformer<T, D>;            ///< The expression type
    using sub_expr_t = std::decay_t<T>;                             ///< The sub expression type
    using value_type = typename etl_traits<sub_expr_t>::value_type; ///< The value type of this expression

    static constexpr bool is_etl                  = true;                                            ///< Indicates if the type is an ETL expression
    static constexpr bool is_transformer          = true;                                            ///< Indicates if the type is a transformer
    static constexpr bool is_view                 = false;                                           ///< Indicates if the type is a view
    static constexpr bool is_magic_view           = false;                                           ///< Indicates if the type is a magic view
    static constexpr bool is_fast                 = false;                                           ///< Indicates if the expression is fast
    static constexpr bool is_linear               = false;                                           ///< Indicates if the expression is linear
    static constexpr bool is_thread_safe                 = etl_traits<sub_expr_t>::is_thread_safe;                 ///< Indicates if the expression is thread safe
    static constexpr bool is_value                = false;                                           ///< Indicates if the expression is of value type
    static constexpr bool is_direct               = false;           ///< Indicates if the expression has direct memory access
    static constexpr bool is_generator            = false;                                           ///< Indicates if the expression is a generated
    static constexpr bool is_padded               = false;                          ///< Indicates if the expression is padded
    static constexpr bool is_aligned               = false;                          ///< Indicates if the expression is padded
    static constexpr bool is_temporary = etl_traits<sub_expr_t>::is_temporary; ///< Indicaes if the expression needs an evaluator visitor
    static constexpr order storage_order          = etl_traits<sub_expr_t>::storage_order;           ///< The expression storage order

    static constexpr size_t sub_d = etl_traits<sub_expr_t>::dimensions(); ///< The number of dimensions of the sub type

    /*!
     * \brief Indicates if the expression is vectorizable using the
     * given vector mode
     * \tparam V The vector mode
     */
    template <vector_mode_t V>
    using vectorizable = std::false_type;

    /*!
     * \brief Returns the size of the given expression
     * \param v The expression to get the size for
     * \returns the size of the given expression
     */
    static size_t size(const expr_t& v) {
        return v.m * etl_traits<sub_expr_t>::size(v.sub);
    }

    /*!
     * \brief Returns the dth dimension of the given expression
     * \param v The expression
     * \param d The dimension to get
     * \return The dth dimension of the given expression
     */
    static size_t dim(const expr_t& v, size_t d) {
        return d < sub_d ? etl_traits<sub_expr_t>::dim(v.sub, d) : v.reps[d - sub_d];
    }

    /*!
     * \brief Returns the number of expressions for this type
     * \return the number of dimensions of this type
     */
    static constexpr size_t dimensions() {
        return D + etl_traits<sub_expr_t>::dimensions();
    }
};

/*!
 * \brief Specialization for dyn_rep_l_transformer
 */
template <typename T, size_t D>
struct etl_traits<dyn_rep_l_transformer<T, D>> {
    using expr_t     = etl::dyn_rep_l_transformer<T, D>;            ///< The expression type
    using sub_expr_t = std::decay_t<T>;                             ///< The sub expression type
    using value_type = typename etl_traits<sub_expr_t>::value_type; ///< The value type of this expression

    static constexpr bool is_etl                  = true;                                            ///< Indicates if the type is an ETL expression
    static constexpr bool is_transformer          = true;                                            ///< Indicates if the type is a transformer
    static constexpr bool is_view                 = false;                                           ///< Indicates if the type is a view
    static constexpr bool is_magic_view           = false;                                           ///< Indicates if the type is a magic view
    static constexpr bool is_fast                 = false;                                           ///< Indicates if the expression is fast
    static constexpr bool is_linear               = false;                                           ///< Indicates if the expression is linear
    static constexpr bool is_thread_safe                 = etl_traits<sub_expr_t>::is_thread_safe;                 ///< Indicates if the expression is thread safe
    static constexpr bool is_value                = false;                                           ///< Indicates if the expression is of value type
    static constexpr bool is_direct               = false;           ///< Indicates if the expression has direct memory access
    static constexpr bool is_generator            = false;                                           ///< Indicates if the expression is a generated
    static constexpr bool is_padded               = false;                          ///< Indicates if the expression is padded
    static constexpr bool is_aligned               = false;                          ///< Indicates if the expression is padded
    static constexpr bool is_temporary = etl_traits<sub_expr_t>::is_temporary; ///< Indicaes if the expression needs an evaluator visitor
    static constexpr order storage_order          = etl_traits<sub_expr_t>::storage_order;           ///< The expression storage order

    /*!
     * \brief Indicates if the expression is vectorizable using the
     * given vector mode
     * \tparam V The vector mode
     */
    template <vector_mode_t V>
    using vectorizable = std::false_type;

    /*!
     * \brief Returns the size of the given expression
     * \param v The expression to get the size for
     * \returns the size of the given expression
     */
    static size_t size(const expr_t& v) {
        return v.m * etl_traits<sub_expr_t>::size(v.sub);
    }

    /*!
     * \brief Returns the dth dimension of the given expression
     * \param v The expression
     * \param d The dimension to get
     * \return The dth dimension of the given expression
     */
    static size_t dim(const expr_t& v, size_t d) {
        return d >= D ? etl_traits<sub_expr_t>::dim(v.sub, d - D) : v.reps[d];
    }

    /*!
     * \brief Returns the number of expressions for this type
     * \return the number of dimensions of this type
     */
    static constexpr size_t dimensions() {
        return D + etl_traits<sub_expr_t>::dimensions();
    }
};

} //end of namespace etl
