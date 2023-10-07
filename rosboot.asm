[org 7c00h]

;初始化寄存器
;BOOT SECTION

LOAD_BASE_DIRECTION1 	EQU 0X900
CYCLE_NUM				EQU 10
VEAS_ES 				EQU 0X100
SCRN_MODE_ES 			EQU 0x200
VMODE					EQU 0X0FF2	;颜色数目信息(颜色位数)
SCRNX 					EQU 0X0FF4	;x分辨率地址
SCRNY					EQU 0X0FF8	;y分辨率地址
VRAM					EQU 0X0FFc	;图像缓冲区开始地址

mov ax,0
mov ds,ax
mov es,ax
mov ss,ax
mov sp,7c00h
call boot
call load_system
mov byte [VMODE],8
mov dword [SCRNX],320
mov dword [SCRNY],200
mov dword [VRAM],0xa0000
call scrnmode_select
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

scrnmode_select: ;0x7cc7
	mov dx,0
	mov ax,VEAS_ES
	mov es,ax
	mov di,0
	mov ax,0x4f00
	int 10h
	cmp ax,0x004f
	jne scrn_defult
	mov ax,[es:di+16]
	mov si,[es:di+14]
	mov ds,ax
	mov di,0
	mov ax,SCRN_MODE_ES
	mov es,ax
	modeloop:
		mov cx,[ds:si]
		cmp cx,0xffff 
		je scrn_fin
		mov ax,0x4f01
		int 10h
		cmp ax,0x004f ;0x7cfb
		jne next_mode
		mov al,[es:di+0x19]
		cmp al,8
		jne next_mode
		mov al,[es:di+0x1b]
		cmp al,4
		jne next_mode
		mov ax,[es:di]
		and ax,0x0080
		jz next_mode
		;0x7d18 可用调色板画面模式,x\y都是32位，要修改后16位
		mov ax,ds
		push ax
		mov ax,0
		mov ds,ax
		mov WORD bx,[SCRNX]
		mov ax,[es:di+0x12]
		cmp ax,bx
		jbe next_mode_pre
		mov dx,cx
		mov WORD [SCRNX],ax
		mov ax,[es:di+0x14]
		mov WORD [SCRNY],ax
		;0x7d38 注意这里vram应该是32位，默认后16位为0，如果有错误要修改
		mov  WORD ax,[es:di+0x28+2]
		mov  WORD [VRAM+2],ax
	next_mode_pre:
		pop ax
		mov ds,ax
		mov word ax,[es:di+0x12]
		cmp ax,1024
		jae scrn_fin
	next_mode:
		add si,2
		jmp modeloop
		
	scrn_defult:
		mov al,0x13
		mov ah,0
		int 10h
		ret
	scrn_fin: ;0x7d57
		cmp dx,0
		je scrn_defult
		mov ax,0x4f02
		mov bx,dx
		or bx,0x4000
		int 10h
		cmp ax,0x004f
		jne scrn_defult
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