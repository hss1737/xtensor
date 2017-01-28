/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"
#include "xtensor/xarray.hpp"
#include "xtensor/xrandom.hpp"
#include "xtensor/xindexview.hpp"
#include "xtensor/xbroadcast.hpp"
#include "xtensor/xview.hpp"
#include "test_common.hpp"

namespace xt
{
    using std::size_t;

    TEST(xindexview, indices)
    {
        xarray<double> e = xt::random::rand<double>({3, 3});
        xarray<double> e_copy = e;
        auto v = make_xindexview(e, {{1, 1}, {1, 2}, {2, 2}});
        EXPECT_EQ(e(1, 1), v(0));
        EXPECT_EQ(e(1, 2), v[{1}]);

        std::vector<size_t> idx = {2};
        EXPECT_EQ(e(2, 2), v.element(idx.begin(), idx.end()));

        v += 3;
        auto expected = e_copy(1, 1) + 3;
        EXPECT_EQ(expected, e(1, 1));

        auto t = v + 3;
        EXPECT_EQ((e_copy(1, 1) + 6), t(0));
        EXPECT_EQ((e(1, 1) + 3), t(0));

        xarray<double> as = {3, 3, 3};
        v = as;
        EXPECT_TRUE(all(equal_to(v, as)));
        EXPECT_EQ(3, e(2, 2));
    }

    TEST(xindexview, boolean)
    {
        xarray<double> e = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
        auto v = make_xboolview(e, e > 0);
        EXPECT_EQ(1, v(0));

        v += 2;
        EXPECT_EQ(3, e(1, 1));
        EXPECT_EQ(3, v(1));

        v += xarray<double>{1, 2, 3};
        EXPECT_EQ(5, e(1, 1));
        EXPECT_EQ(6, e(2, 2));

        xarray<double> e2 = random::rand<double>({3, 3, 3, 3});
        auto v2 = make_xboolview(e2, e2 > 0.5);
        v2 *= 0;
        EXPECT_TRUE(!any(e2 > 0.5));
    }

}