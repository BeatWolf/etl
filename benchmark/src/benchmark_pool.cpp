//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#define CPM_LIB
#include "benchmark.hpp"

CPM_BENCH() {
    CPM_TWO_PASS_NS_P(
        pmp_policy,
        "pmp_h(c=2) (s) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(smat(d, d), smat(d,d)); },
        [](smat& a, smat& r){ r = etl::p_max_pool_h<2,2>(a); },
        [](std::size_t d){ return 2 * d * d * 2 * 2; }
        );

#ifdef ETL_EXTENDED_BENCH
    CPM_TWO_PASS_NS_P(
        pmp_policy_3,
        "pmp_h_3(c=2) (s) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(smat3(50UL, d, d), smat3(50UL, d,d)); },
        [](smat3& a, smat3& r){ r = etl::p_max_pool_h<2,2>(a); },
        [](std::size_t d){ return 50 * 2 * d * d * 2 * 2; }
        );

    CPM_TWO_PASS_NS_P(
        pmp_policy_3,
        "pmp_h_4(c=2) (s) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(smat4(50UL, 50UL, d, d), smat4(50UL, 50UL, d,d)); },
        [](smat4& a, smat4& r){ r = etl::p_max_pool_h<2,2>(a); },
        [](std::size_t d){ return 50 * 50 * 2 * d * d * 2 * 2; }
        );

    CPM_TWO_PASS_NS_P(
        pmp_policy,
        "dyn_pmp_h(c=2) (s) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(smat(d, d), smat(d, d)); },
        [](smat& a, smat& r){ r = etl::p_max_pool_h(a, 2, 2); },
        [](std::size_t d){ return 2 * d * d * 2 * 2; }
        );
#endif

    CPM_TWO_PASS_NS_P(
        pmp_policy,
        "pmp_p(c=2) (s) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(smat(d, d), smat(d/2,d/2)); },
        [](smat& a, smat& r){ r = etl::p_max_pool_p<2,2>(a); },
        [](std::size_t d){ return 2 * d * d * 2 * 2; }
        );

    CPM_TWO_PASS_NS_P(
        pmp_policy,
        "pmp_h(c=4) (s) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(smat(d, d), smat(d,d)); },
        [](smat& a, smat& r){ r = etl::p_max_pool_h<4,4>(a); },
        [](std::size_t d){ return 2 * d * d * 4 * 4; }
        );

    CPM_TWO_PASS_NS_P(
        pmp_policy,
        "pmp_p(c=4) (s) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(smat(d, d), smat(d/4,d/4)); },
        [](smat& a, smat& r){ r = etl::p_max_pool_p<4,4>(a); },
        [](std::size_t d){ return 2 * d * d * 4 * 4; }
        );

    CPM_TWO_PASS_NS_P(
        pmp_policy,
        "pmp_h(c=2) (d) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(dmat(d, d), dmat(d,d)); },
        [](dmat& a, dmat& r){ r = etl::p_max_pool_h<2,2>(a); },
        [](std::size_t d){ return 2 * d * d * 2 * 2; }
        );

    CPM_TWO_PASS_NS_P(
        pmp_policy,
        "pmp_p(c=2) (d) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(dmat(d, d), dmat(d/2,d/2)); },
        [](dmat& a, dmat& r){ r = etl::p_max_pool_p<2,2>(a); },
        [](std::size_t d){ return 2 * d * d * 2 * 2; }
        );

    CPM_TWO_PASS_NS_P(
        pmp_policy,
        "pmp_h(c=4) (d) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(dmat(d, d), dmat(d,d)); },
        [](dmat& a, dmat& r){ r = etl::p_max_pool_h<4,4>(a); },
        [](std::size_t d){ return 2 * d * d * 4 * 4; }
        );
CPM_TWO_PASS_NS_P(
        pmp_policy,
        "pmp_p(c=4) (d) [pmp][s]",
        [](std::size_t d){ return std::make_tuple(dmat(d, d), dmat(d/4,d/4)); },
        [](dmat& a, dmat& r){ r = etl::p_max_pool_p<4,4>(a); },
        [](std::size_t d){ return 2 * d * d * 4 * 4; }
        );
}
