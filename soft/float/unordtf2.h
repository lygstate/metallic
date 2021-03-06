/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_UNORDTF2_H
#define METALLIC_UNORDTF2_H

#include "isnanl.h"

static int unordtf2(long double x, long double y)
{
    return __isnanl(x) || __isnanl(y);
}

#endif
