[org 7c00h]

;初始化寄存器
;BOOT SECTION
;SECTION 1
; LOAD_SECTION1 EQU 2
; LOAD_SECTION_NUM1 EQU 17
LOAD_BASE_DIRECTION1 EQU 0X900
CYCLE_NUM EQU 10
VEAS_ES EQU 0X100
; LOAD_SECTION2 EQU 1
; LOAD_SECTION_NUM2 EQU 18
; LOAD_BASE_DIRECTION2 EQU 0XB20
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
	;读取10柱面，共179.5KB rosboot.bin不超过0x2D000
	mov ax,LOAD_BASE_DIRECTION1
	mov es,ax
	mov cx,0002h 	;柱面0 扇区2
	mov dh,0		;磁头0
testloop:
	mov si,0		;读取失败次数
	try:
		mov ah,2
		mov al,1 	;1个扇区
		mov bx,0
		mov dl,0
		int 13h
		jnc next
		add si,1
		cmp si,5
		jae catch
		mov ah,0
		mov dl,0
		int 13h
		jmp try
	next:
		mov ax,es
		add ax,20h
		mov es,ax
		add cl,1
		cmp cl,18
		jbe testloop
		mov cl,1
		add dh,1
		cmp dh,2
		jb testloop
		mov dh,0
		add ch,1
		cmp ch,CYCLE_NUM
		jbe testloop
		
	finish:
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

	catch:
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

mov ax,VEAS_ES
mov es,ax
mov di,0
mov ax,0x4f00
int 10h
cmp ax,0x004f
jne scrn_defult

mov ax,ds:
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