/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "copy.h"

void* __mempcpy(void* restrict destination, const void* restrict source, size_t length)
{
    return copy(destination, source, length);
}

void* memcpy(void* restrict destination, const void* restrict source, size_t length)
{
    copy(destination, source, length);
    return destination;
}
