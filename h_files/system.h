#ifndef _SYSTEMH_
#define _SYSTEMH_
#include "ros.h"
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1
#define PORT_GETKEY		0X0060
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x8e00
#define AR_TSS32		0x0089
// struct define
struct GDT_SEG
{
	short limit_low;
	short base_low;
	char base_mid;
	char access_right;
	char limit_high_pageset;
	char base_high;
};

struct IDT_INTGATE
{
	short offset_low;
	short selector;
	short settings;
	short offset_high;
};

// global define
struct GDT_SEG* gs  = (struct GDT_SEG*)0x00500000;
struct IDT_INTGATE* ii = (struct IDT_INTGATE*)0x00510000;

// function define
void set_gdt_segment(struct GDT_SEG *gs, unsigned int limit, unsigned int base, char ar,int type);
void set_idt_segment(struct IDT_INTGATE* ii, unsigned int offset,unsigned int selector, int settings);
void init_PIC();
void init_PIT();
void wait_kbc_ready();
void init_keyboard();
void enable_mouse();
#endif