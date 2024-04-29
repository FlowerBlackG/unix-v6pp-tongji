#ifndef TYPES_H
#define TYPES_H

#include "../stdint.h"

/*
	����ʱ�䣺2:44 PM 2008-8-9
	˵���������������͵�typedef
*/

/*
	���¶������������ֽڳ���Ϊ��gcc����sizeof��õĽ��
*/
typedef unsigned char	uchar;		/*1���ֽڳ�*/
typedef unsigned char	BYTE;		/*1���ֽڳ�*/
typedef unsigned char	UCHAR;		/*1���ֽڳ�*/

typedef unsigned short	ushort;		/*2���ֽڳ�*/
typedef unsigned short	WORD;		/*2���ֽڳ�*/
typedef unsigned short	USHORT;		/*2���ֽڳ�*/

typedef unsigned int	uint;		/*4���ֽڳ�*/
typedef unsigned int	DWORD;		/*4���ֽڳ�*/
typedef unsigned int	UINT;		/*4���ֽڳ�*/

/*
	���������һ�׶���ȽϺã��ĸ��������Ƚϡ����桰�������� 

	    GTY ���Ҿ��� ../stdint.h ����ĸ����� :D
*/



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

#endif
