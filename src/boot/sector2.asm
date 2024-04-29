[BITS 32]
[extern kernelBridge]


global greatstart
greatstart:
	mov eax,1
	mov eax,2
	mov eax,3

	jmp kernelBridge
	ud2
