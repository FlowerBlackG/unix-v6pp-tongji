/* 内核构建链接脚本。 */

ENTRY(greatstart)

SECTIONS
{
    
    .text 0xc0100000:
    {
        boot/sector2.o(.text)
        *(.text)
        *(.rodata*)
        . = ALIGN(4096);
    }
    
    .data :
    {
        
        *(.data)
        . = ALIGN(4096);

        /* 构造函数列表。
           参考：https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/text/ld.txt */
        __CTOR_LIST__ = .;
        *(.init_array)
        __CTOR_END__ = .;

        . = ALIGN(4096);

        /* 析构函数列表。 */
        __DTOR_LIST__ = .;
        *(.dtors)
        __DTOR_END__ = .;

        . = ALIGN(4096);
    }


    .bss : 
    {
        __BSS_START__ = .;
     
        *(.bss)
        . = ALIGN(4096);

        __BSS_END__ = .;
    }

}
