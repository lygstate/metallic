/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

double nextafter(double from, double to)
{
    if (from == to || to != to)
        return to;

    if (from != from)
        return from;

    if (from == 0)
        return copysign(__DBL_DENORM_MIN__, to);

    int64_t d = reinterpret(int64_t, from);
    int64_t a = reinterpret(int64_t, to);

    a < d || (a ^ d) < 0 ? --d : ++d;

    return reinterpret(double, d);
}
