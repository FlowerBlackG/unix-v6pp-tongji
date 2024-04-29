#ifndef TYPES_H
#define TYPES_H

#include "../stdint.h"

/*
	创建时间：2:44 PM 2008-8-9
	说明：对于数据类型的typedef
*/

/*
	以下定义数据类型字节长度为在gcc中用sizeof获得的结果
*/
typedef unsigned char	uchar;		/*1个字节长*/
typedef unsigned char	BYTE;		/*1个字节长*/
typedef unsigned char	UCHAR;		/*1个字节长*/

typedef unsigned short	ushort;		/*2个字节长*/
typedef unsigned short	WORD;		/*2个字节长*/
typedef unsigned short	USHORT;		/*2个字节长*/

typedef unsigned int	uint;		/*4个字节长*/
typedef unsigned int	DWORD;		/*4个字节长*/
typedef unsigned int	UINT;		/*4个字节长*/

/*
	你觉得用那一套定义比较好，哪个看起来比较”正规“。。。？ 

	    GTY 答：我觉得 ../stdint.h 里面的更舒适 :D
*/



/** 结构体紧凑。 */
#ifndef __packed
    #define __packed __attribute__((packed))
#endif

#ifndef __no_return
    #define __no_return __attribute__((__noreturn__))
#endif

/** 令函数不保存 rsp 和 rbp。 */
#ifndef __omit_frame_pointer
    #define __omit_frame_pointer __attribute__((optimize("omit-frame-pointer")))
#endif

/** 强制内联。 */
#ifndef __force_inline
    #define __force_inline __attribute__((always_inline)) inline
#endif

/** 内联汇编。 */
#ifndef __asm
    #define __asm __asm__ __volatile__
#endif

/** 指定代码存放的区域。 */
#ifndef __section
    #define __section(name) __attribute__((section(name)))
#endif

typedef unsigned long size_t;

typedef int32_t pid_t;

#endif
