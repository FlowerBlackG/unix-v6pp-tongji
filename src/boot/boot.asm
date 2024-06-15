org 0x7c00


; 
; 实模式内存布局
;
; 起始地址    大小       用途
; ----------------------------------------
; 0x000      1KB       中断向量表
; 0x400      256B      BIOS 数据区
; 0x500      29.75KB   可用区域
; 0x7C00     512B      MBR 加载区域
; 0x7E00     607.6KB   可用区域
; 0x9FC00    1KB       扩展 BIOS 数据区
; 0xA0000    64KB      用于彩色显示适配器
; 0xB0000    32KB      用于黑白显示适配器
; 0xB8000    32KB      用于文本显示适配器
; 0XC0000    32KB      显示适配器 BIOS
; 0XC8000    160KB     映射内存
; 0xF0000    64KB-16B  系统BIOS
; 0xFFFF0    16B       系统 BIOS 入口地址
; 



; vesa support
;
;   https://github.com/FlowerBlackG/YurongOS/blob/master/src/boot/boot.asm
;   added by GTY
vesa_video_mode equ 0x143
vesa_video_mode_code equ (vesa_video_mode | 0x4000)


;section .code16
[BITS 16]
start:

%ifdef USE_VESA
		; 读取 VESA 信息。
		xor ax, ax
		mov es, ax
		mov di, 0x7e00
		mov ax, 0x4f01
		mov cx, vesa_video_mode
		int 0x10

		; 设置屏幕模式为文本模式，并清空屏幕。
		; 中断指令号为 10H，当 AH=0H 时表示设置显示模式，模式具体为 AL。
		; AL=3H 表示文本模式，80×25，16色。
		; AL=12H 表示图形模式，VGA 640×480 16色
		; AX=0x4F02, BX=0x4180 表示 1440×900 32位色
		; AX=0x4F02, BX=0x4143 表示 800×600 32位色

		mov bx, vesa_video_mode_code
		mov ax, 0x4F02
		int 0x10
%endif


		lgdt [gdtr]
		
		cli

		;打开a20 地址线
		in al,92h
		or al,00000010b
		out 92h, al

;		start to load sector 1 to memory 
		
		mov eax, cr0;
		or eax, 1;
		mov cr0, eax

		; enable PSE so we can use 2MB page :D  -- added by gty
		; See:
		;   https://www.wikiwand.com/en/Control_register#CR4
		;   https://wiki.osdev.org/Paging
		mov eax, cr4
		or eax, 0b10000
		mov cr4, eax
				
		jmp dword 0x8:_startup ;

	
;section .code32
[BITS 32]
_startup:

		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov ss, ax

		mov	ecx, KERNEL_SIZE 	;cx = 扇区数KERNEL_SIZE，作为loop的次数
		mov eax, 1				;LBA寻址模式下sector编号从0开始。  #0是引导扇区，#1扇区开始才是kernel的首扇区
		mov ebx, 0x100000		;目标存放地址从1M处开始，每次loop递增512 bytes
_load_kernel:
		push eax
		inc eax
		
		push ebx
		add	ebx, 512
		call _load_sector
		loop _load_kernel		
		
		;修改所有寄存器到高位地址
		mov ax, 0x20
		mov ds, ax
		mov es, ax
		mov ss, ax
		or esp, 0xc0000000
		jmp 0x18:0xc0100000
		
_load_sector:
	push ebp
	mov ebp,esp
	
	push edx
	push ecx
	push edi
	push eax		
	
	mov al,1		;读1个扇区
	mov dx,1f2h		;扇区数寄存器 0x1f2
	out dx,al
	
	mov eax,[ebp+12] ;[ebp+12]对应上面mov eax, 1   push eax指令入栈的值，eax为要读入的扇区号
					;LBA28(Linear Block Addressing)模式输入扇区号的Bits 7~0， 共28 Bits扇区号
	inc dx			;扇区号寄存器 0x1f3
	out dx,al
	
	shr eax,8		;LBA28(Linear Block Addressing)模式输入扇区号的Bits 15~8 放入AL中， 共28 Bits扇区号
	inc dx			;Port：DX = 0x1f3+1 = 0x1f4  
	out dx,al
	
	shr eax,8		;LBA28(Linear Block Addressing)模式输入扇区号的Bits 23~16放入AL中， 共28 Bits扇区号
	inc dx			;Port：DX = 0x1f4+1 = 0x1f5 
	out dx,al
	
	shr eax,8
	and al,0x0f
	or al,11100000b ;Bit(7和5)为1表示是IDE接口，Bit(6)为1表示开启LBA28模式，Bit(4)为1表示主盘。
					;Bit(3~0)为LBA28中的Bit27~24位
	inc dx			;Port：DX = 0x1f5+1 = 0x1f6 
	out dx,al
	
	mov al,0x20		;0x20表示读1个sector，0x30表示写1个sector
	inc dx			;Port：DX = 0x1f6+1 = 0x1f7 
	out dx,al
	
.test:
	in al,dx
	test al,10000000b
	jnz .test
	
	test al,00001000b
	jz .load_error
	
	
	mov ecx,512/4
	mov dx,0x1f0
	mov edi,[ebp+8]	;取得call前入栈参数[ebp+8] = 0x100000  = 1MB
	rep insd
	xor ax,ax
	jmp .load_exit
	
.load_error:
	mov dx,0x1f1
	in al,dx
	xor ah,ah
			
.load_exit:
	
	pop eax		
	pop edi
	pop ecx
	pop edx
	leave		;Destory stack frame
	retn 8		
		
;section .data
KERNEL_SIZE		equ		(398)	    

gdt:		
		dw	0x0000
		dw	0x0000
		dw	0x0000
		dw	0x0000
		
		dw	0xFFFF		
		dw	0x0000		
		dw	0x9A00		
		dw	0x00CF		
		
		dw	0xFFFF		
		dw	0x0000		
		dw	0x9200		
		dw	0x00CF		
		
		dw	0xFFFF		
		dw	0x0000		
		dw	0x9A00		
		dw	0x40CF		
		
		dw	0xFFFF		
		dw	0x0000		
		dw	0x9200		
		dw	0x40CF		
		
gdtr:
		dw $-gdt		;limit
		dd gdt			;offset

		times 510 - ($ - $$) db 0
		dw 0xAA55
