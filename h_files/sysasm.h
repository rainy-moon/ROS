#ifndef _SYSASMH_
#define _SYSASMH_
#include "ros.h"
void hlt();
void sti();
void cli();
void stihlt();
int store_eflags();
void load_eflags(int eflags);
void io_out8(int port,int data);
void io_out16(int port,int data);
void io_out32(int port,int data);
unsigned char io_in8(int port);
unsigned short io_in16(int port);
unsigned int io_in32(int port);
void load_gdt(int limit,int addr);
void load_idt(int limit,int addr);
void load_tr(int tr);
void taskchange(int eip,int cs);//farjump
void asm_inthandler20h();
void asm_inthandler21h();
void asm_inthandler27h();
void asm_inthandler2ch();
#endif