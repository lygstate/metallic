/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "expf_pade.h"
#include <math.h>
#include <stdint.h>

static double _taylor(double x)
{
    const double c3 = 1.6666666666666666667e-1; /* 1 / 3! */
    const double c5 = 8.3333333333333333333e-3; /* 1 / 5! */
    const double c7 = 1.9841269841269841270e-4; /* 1 / 7! */

    double xx = x * x;

    return x + x * xx * (c3 + xx * (c5 + xx * c7));
}

float sinhf(float x)
{
    const float maximum = 88.72283935546875;
    const float log2e = 1.44269502163;
    const double ln2 = 0.6931471805599453094;

    if (fabsf(x) > maximum)
        return x * HUGE_VALF;

    float n = nearbyintf(x * log2e);

    if (n == 0)
        return _taylor(x);

    double y = expf_pade(x - n * ln2);
    int64_t shifted = *(int64_t*)&y + ((int64_t) n << 52);

    y = *(double*)&shifted;

    return 0.5 * y - 0.5 / y;
}
