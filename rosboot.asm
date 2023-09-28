[org 7c00h]

;初始化寄存器
;BOOT SECTION
;SECTION 1
LOAD_SECTION1 EQU 2
LOAD_SECTION_NUM1 EQU 17
LOAD_BASE_DIRECTION1 EQU 0X900
LOAD_SECTION2 EQU 1
LOAD_SECTION_NUM2 EQU 18
LOAD_BASE_DIRECTION2 EQU 0XB20
mov ax,0
mov ds,ax
mov es,ax
mov ss,ax
mov sp,7c00h
call boot
call load_system
jmp 9000h

boot:
	mov ax,bootmessage1
	mov bp,ax
	mov cx,18
	mov ax,1301h
	mov dx,0
	mov bx,000ch
	int 10h
	ret

load_system:
	mov ax,bootmessage2
	mov bp,ax
	mov cx,19
	mov ax,1301h
	mov dx,0100h
	mov bx,000ch
	int 10h
	mov si,0 ;记录尝试读取次数
	try1:
		;先读柱面0 磁头0 2-18扇区 8.5K 101000 - 103200
		mov ax,LOAD_BASE_DIRECTION1
		mov es,ax
		mov ah,02h
		mov al,LOAD_SECTION_NUM1
		mov bx,0
		mov ch,0
		mov cl,LOAD_SECTION1
		mov dx,0
		int 13h
		test ah,0
		je finish1
		add si,1
		cmp si,5
		jnb error
		mov ah,0
		mov dl,0
		int 13h ;磁盘复位
		jmp try1
	finish1:
		mov ah,0
		mov dl,0
		int 13h ;磁盘复位
		mov si,0
	try2:
		;0 1 1-18 9K
		mov ax,LOAD_BASE_DIRECTION2
		mov es,ax
		mov ah,02h
		mov al,LOAD_SECTION_NUM2
		mov bx,0
		mov ch,0
		mov cl,LOAD_SECTION2
		mov dx,10h
		int 13h
		test ah,0
		je finish2
		add si,1
		cmp si,5
		jnb error
		mov ah,0
		mov dl,0
		int 13h ;磁盘复位
		jmp try2
	finish2:
		mov ax,0
		mov es,ax
		mov ax,bootmessage3
		mov bp,ax
		mov cx,10
		mov ax,1301h
		mov dx,0200h
		mov bx,000ch
		int 10h
		ret
	error:
		mov ax,0
		mov es,ax
		mov ax,booterror1
		mov bp,ax
		mov cx,23
		mov ax,1301h
		mov dx,0200h
		mov bx,000ch
		int 10h
		jmp $
		ret

bootmessage1: db 'STARTING BOOT...',0ah,0dh
bootmessage2: db 'Loading System...',0ah,0dh
bootmessage3: db 'Success!',0ah,0dh 
booterror1: db 'Loading System ERROR!',0ah,0dh

TIMES 510 - ($ - $$) DB 0
dw 0xAA55

;SYSTEM SECTION
;SECTION 2 & OTHERS

mov ax,0
mov es,ax
mov ax,0x901a
mov bp,ax
mov cx,20
mov ax,1301h
mov dx,0500h
mov bx,000ch
int 10h
jmp $
sys_message1: db 'hello_world in asm',0ah,0dh
TIMES 1474560 - ($ - $$) DB 0