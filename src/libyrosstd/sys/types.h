// SPDX-License-Identifier: MulanPSL-2.0

/*
 * �������Ͷ��塣
 * ������ 2022��7��2�ա�
 */

#pragma once

#include "../stdint.h"

/** �ļ���β�� */
#ifndef EOF
    #define EOF -1
#endif

/** ��ָ�롣 */
#ifndef NULL
    #define NULL 0
#endif

/** �ṹ����ա� */
#ifndef __packed
    #define __packed __attribute__((packed))
#endif

#ifndef __no_return
    #define __no_return __attribute__((__noreturn__))
#endif

/** ��������� rsp �� rbp�� */
#ifndef __omit_frame_pointer
    #define __omit_frame_pointer __attribute__((optimize("omit-frame-pointer")))
#endif

/** ǿ�������� */
#ifndef __force_inline
    #define __force_inline __attribute__((always_inline)) inline
#endif

/** ������ࡣ */
#ifndef __asm
    #define __asm __asm__ __volatile__
#endif

/** ָ�������ŵ����� */
#ifndef __section
    #define __section(name) __attribute__((section(name)))
#endif

typedef unsigned long size_t;

typedef int32_t pid_t;
