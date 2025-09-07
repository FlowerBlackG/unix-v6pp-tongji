; 本文件经过混淆，不宜直接阅读。也没必要尝试破解。
; 有疑惑请咨询老师。如果特别好奇，别急，这可能是下学期的作业哦～～


[BITS 32]
[extern kernelBridge]


global greatstart
greatstart:

db 0xbf, 0x00, 0x10, 0x00, 0xc0, 0xb9, 0x00, 0x04, 0x00, 0x00

call ___q98w_ddx


db 0xb8, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x83, 0x00, 0x00, 0x00, 0xa3, 0x00, 0x1c, 0x00, 0xc0, 0xa3, 0x00, 0x10, 0x00, 0xc0

; load empty idt
lidt [_____jsdl]
db 0xba, 0x00, 0x10, 0x00, 0x00, 0x0f, 0x22, 0xda


lgdt [__iuhurwuie8]


db 0x0f, 0x20, 0xc3, 0x81, 0xcb, 0x01, 0x00, 0x00, 0x80, 0x0f, 0x22, 0xc3


jmp _xxl:.x87_32j

.x87_32j:
db 0xbc, 0x00, 0x7c, 0x00, 0xc0

jmp kernelBridge
ud2



___q98w_ddx:

db 0x31, 0xc0

._0001:

mov dword [edi], eax
db 0x83, 0xc7, 0x04, 0x49, 0x83, 0xf9, 0x00

jne ._0001

db 0xc3



align 4
_____jsdl:
dw 0
dd 0


; 选择子。
_xxl equ (1 << 3)


align 4

__iuhurwuie8:
dw (_ds80 - _d83j) - 1 ; limit

dd _d83j ; base
dd 0

align 4

_d83j:
dq 0


gdt_code:

; (idx)    : 1
; limit    : 0xfffff
; base     : 0x0
; access   : 0
; rw       : 1
; dc       : 0
; exec     : 1
; descType : code/data
; privi lv : 0
; present  : 1
; longMode : 0
; sizeFlag : 32 bits
; granular : 4 KB

dq 0xcf9a000000ffff

gdt_data:

; (idx)    : 2
; limit    : 0xfffff
; base     : 0x0
; access   : 0
; rw       : 1
; dc       : 0
; exec     : 0
; descType : code/data
; privi lv : 0
; present  : 1
; longMode : 0
; sizeFlag : 32 bits
; granular : 4 KB

dq 0xcf92000000ffff


_ds80:
