// SPDX-License-Identifier: MulanPSL-2.0

/*
 * �ɱ������
 * ������ 2022��7��13�ա�
 * 
 * �ο���
 *   https://pubs.opengroup.org/onlinepubs/9699919799/
 *   https://cplusplus.com/reference/cstdarg/
 */

#pragma once

#include <libyrosstd/sys/types.h> // for NULL.

#if (__x86_64__)

// �ο� linux5.19 include/linux/stdarg.h
// ԭ�� gcc �������Ѿ�ʵ���������... ������˼��������ôд...

typedef __builtin_va_list va_list;

#define va_start(ap, argN) __builtin_va_start(ap, argN)

#define va_copy(dest, src) __builtin_va_copy(dest, src)

#define va_arg(ap, type) __builtin_va_arg(ap, type)

#define va_end(ap) __builtin_va_end(ap)

#elif (__i386__)

typedef uint32_t va_list;

#define va_start(ap, argN) (ap = (va_list) &argN + sizeof(argN))

#define va_copy(dest, src) (dest = src)

#define va_arg(ap, type) (*(type*)((ap += sizeof(void*)) - sizeof(void*)))

#define va_end(ap) (ap = (va_list) NULL)

// if yros, then emit: #warning x86_32 mode is not recommended.  

#else

#error cpu arch not supported.

#endif
