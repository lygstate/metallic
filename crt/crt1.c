/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>
#include <stdlib.h>

void* sbrk(intptr_t);
int main(void);

_Noreturn void _start(void)
{
    sbrk((intptr_t)__builtin_frame_address(0));
    exit(main());
}
