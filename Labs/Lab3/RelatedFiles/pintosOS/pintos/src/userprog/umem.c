/* 
 * This file is derived from source code for the Pintos
 * instructional operating system which is itself derived
 * from the Nachos instructional operating system. The 
 * Nachos copyright notice is reproduced in full below. 
 *
 * Copyright (C) 1992-1996 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose, without fee, and
 * without written agreement is hereby granted, provided that the
 * above copyright notice and the following two paragraphs appear
 * in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
 * AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
 * BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 *
 * Modifications Copyright (C) 2017-2018 David C. Harrison. 
 * All rights reserved.
 */

#include <stdio.h>

#include "threads/vaddr.h"
#include "userprog/umem.h"

static void throw_segmantation_fault(void)
{
    printf("[FATAL ERROR] Segmentation Fault!\n");
    ASSERT(false);
}

void umem_check(const uint8_t *uaddr)
{
    if (umem_get(uaddr) == -1) {
        throw_segmantation_fault();
    }
}

int32_t umem_get(const uint8_t *uaddr)
{
    if (!((void*) uaddr < PHYS_BASE)) {
        return -1;
    }
    int result;
    asm("movl $1f, %0; movzbl %1, %0; 1:" : "=&a" (result) : "m" (*uaddr));
    return result;
}

bool umem_put(uint8_t *udst, uint8_t byte)
{
    if (!((void*) udst < PHYS_BASE)) {
        return false;
    }
    int error_code;
    asm("movl $1f, %0; movb %b2, %1; 1:" : "=&a" (error_code), "=m" (*udst) : "q" (byte));
    return error_code != -1;
}

int umem_read(void *src, void *dst, size_t bytes)
{
    int32_t value;
    size_t i;
    for (i = 0; i < bytes; i++) {
        value = umem_get(src + i);
        if (value == -1)
            throw_segmantation_fault();
        *(char*) (dst + i) = value & 0xff;
    }
    return (int) bytes;
}
