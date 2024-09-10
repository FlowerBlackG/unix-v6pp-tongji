[BITS 32]
[extern kernelBridge]


pml2_address equ 0x1000


global greatstart
greatstart:

	; fill pml2 with 0
	mov edi, (pml2_address + 0xc0000000)
	mov ecx, (0x1000 / 4)
	call zero_fill_area

	; map pml2's [3G, 3G+4M) to [0, 4M)
	mov eax, 0x0
	or eax, 0b000010000011  ; page-size(4MB), read-write, present
	mov [pml2_address + (768 * 4) + 0xc0000000], eax
	mov [pml2_address + 0xc0000000], eax

	; load empty idt
	lidt [empty_idt]

	; set page table addr to pml2_addr
	mov edx, pml2_address
	mov cr3, edx

	
	lgdt [gdt_pointer]


	; enable paging and protect mode
	mov ebx, cr0
	or ebx, 0x80000001
	mov cr0, ebx

	jmp code_selector:.to_kernel_bridge

.to_kernel_bridge:
	mov esp, 0xc0007c00
	jmp kernelBridge
	ud2


; 用 0 填充一个页框。
; 函数内不备份任何可能用到的寄存器。
;
; 传参：
;   edi: 起始地址。
;   ecx: 单元数。以 4 字节为 1 个单元。
;        必须大于 0。函数内不做正确性校验。
;
zero_fill_area:

    xor eax, eax

.zero_fill_dword:

    mov dword [edi], eax
    add edi, 4
    dec ecx

    cmp ecx, 0
    jne .zero_fill_dword

    ret



align 4 ; 4 字节对齐
empty_idt:
    .length dw 0
    .base dd 0


; 选择子。
code_selector equ (1 << 3)
data_selector equ (2 << 3)

align 4

gdt_pointer:
    dw (gdt_end - gdt_base) - 1 ; limit
    
    dd gdt_base ; base
    dd 0

align 4

gdt_base:
    dq 0

; 代码段。
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
    

gdt_end:
