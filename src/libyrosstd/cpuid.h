// SPDX-License-Identifier: MulanPSL-2.0

/*
 * CPU ID Header
 *
 * ������ 2023��7��3�� �Ϻ��мζ�����ͤ��
 */

#pragma once

#include <libyrosstd/sys/types.h>

#define __cpuid(level, a, b, c, d) \
    __asm ( \
        "cpuid \n\t" \
        : "=a" (a), "=b" (b), "=c" (c), "=d" (d) \
        : "0" (level) /* Ϊʲô�� "0"? ��û�������� gcc �� llvm ������ô�ɵġ��ְ�... */ \
    )

#define __cpuid(level, count, a, b, c, d) \
    __asm ( \
        "cpuid \n\t" \
        : "=a" (a), "=b" (b), "=c" (c), "=d" (d) \
        : "0" (level), "2" (count) \
    )


#define cpuid __cpuid

