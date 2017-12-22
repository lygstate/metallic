/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "prec/cisf.h"
#include "prec/expf.h"
#include <complex.h>
#include <math.h>

inline float _Complex __ctanhf(float x, float y)
{
    if (isinf(x))
        return CMPLXF(copysignf(1, x), copysignf(0, y));

    if (x != x)
        return CMPLXF(x, y ? x : y);

    double _Complex circular = __prec_cisf(2.0 * y);

    double exp = __prec_expf(2 * x);
    double cosh = 0.5 * exp + 0.5 / exp;
    double sinh = 0.5 * exp - 0.5 / exp;

    return CMPLX(sinh, cimag(circular)) / (cosh + (double)circular);
}
