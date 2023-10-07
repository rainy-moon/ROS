[org 0x9000]
;图像设置信息，没有实际作用
CYLS 	EQU 0x0ff0	;?
LEDS 	EQU 0X0FF1	;获得键盘led状态
VMODE	EQU 0X0FF2	;颜色数目信息(颜色位数)
SCRNX 	EQU 0X0FF4	;x分辨率地址
SCRNY	EQU 0X0FF8	;y分辨率地址
VRAM	EQU 0X0FFc	;图像缓冲区开始地址
FL_BOOT	EQU 0x00100000;主引导模块转移地址 512B
FL_W2D  EQU 0x00100200;32位转化程序 512B
FL_ROSINIT EQU 0x00101000;system init,screen init .etc 1M
STACK	EQU 0x003fffff;栈底 1M
GDTable EQU 0x00500000;GDT 64k
IDTable EQU 0x00510000;LDT 2K
; _w2dstart:
; mov al,0x13
; mov ah,0
; int 10h
; mov byte [VMODE],8
; mov dword [SCRNX],320
; mov dword [SCRNY],200
; mov dword [VRAM],0xa0000
mov ax,0
mov ds,ax
mov es,ax
mov ss,ax
		
mov ah,2
int 16h
mov byte [LEDS],al

;软中段0打开a20gate
mov ax,2401h
int 15h

;关闭PCI主从中断
mov al,0xff
out 0x21,al
NOP					;有的机器不支持连续out
out 0xa1,al
CLI					;关闭cpu中断响应

; ;打开a20gate
; ;操作键盘控制器打开
; call waitinbufept
; mov al,0adh
; out 64h,al
; call waitinbufept
; mov al,0d0h
; out 64h,al
; call waitoutbufept
; in al,dx
; push ax
; call waitinbufept
; mov al,0d1h
; out 64h,al
; call waitinbufept
; pop ax
; or al,02h
; out 60h,al
; call waitinbufept
; mov al,0aeh
; out 64h,al
; jmp fast_a20
; waitinbufept:
; 	in al,64h
; 	test al,02h
; 	jnz waitinbufept
; 	ret

; waitoutbufept:
; 	in al,64h
; 	test al,1
; 	jz waitoutbufept
; 	ret
; CALL	waitkbdout
; MOV		AL,0xd1
; OUT		0x64,AL
; CALL	waitkbdout
; MOV		AL,0xdf			; enable A20
; OUT		0x60,AL
; CALL	waitkbdout

;操作altermate a20gate 打开
fast_a20:
in al,92h
or al,2
out 92h,al



lgdt [GDTR0]


mov eax,cr0
and eax,0x7fffffff
or  eax,0x00000001
mov cr0,eax
jmp clearflush

clearflush:
mov ax,8
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
;mov ax,16
mov ss,ax


; mov al,0
; out 0x21,al
; NOP					;有的机器不支持连续out
; out 0xa1,al

;转移引导区域
mov esi,7c00h
mov edi,FL_BOOT
mov ecx,512/4
call memcpy
;转移系统初始化区域
mov esi,9000h
mov edi,FL_W2D
mov ecx,rosinit
sub ecx,0x9000
shr ecx,2
call memcpy
;转移系统初始化代码 先转移512KB,以后不够再增加转移
mov esi,rosinit
mov edi,FL_ROSINIT
mov ecx,512*1024/4
call memcpy
;转移栈

mov esp,STACK

jmp 2<<3:0
;jmp $



memcpy: ;ESI 原地址,EDI 目的地址,ECX 移动次数,每次移动4字节
	mov eax,[esi]
	mov [edi],eax
	add esi,4
	add edi,4
	loop memcpy
	ret
	
align 16
GDT0:
		TIMES 8 DB 0				
		DW		0xffff,0x0000,0x9200,0x00cf	;4G 0x0
		DW		0xffff,0x1000,0x9a10,0x004f	;1M 0x00101000

		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0

align 32
rosinit:
	
	hlt