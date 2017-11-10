/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atanhf.h"
#include "reducef.h"
#include "quietf.h"
#include <math.h>

static double _finite(double x)
{
    const double ln2_2 = 0.3465735902799726547;

    int exponent;
    double y = reducef((1 + x) / (1 - x), &exponent);

    if (exponent)
        return kernel_atanhf((y - 1) / (y + 1)) + exponent * ln2_2;
    else
        return kernel_atanhf(x);
}

float atanhf(float x)
{
    float absx = fabsf(x);

    if (absx == 1)
        return copysignf(HUGE_VALF, x);

    if (absx < 1)
        return _finite(x);

    return __quietf(x);
}
