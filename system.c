#include"ros.h"
//系统函数
/**
 * @brief Set the gdt segment object
 * change gs directly without return
 * @param gs 表项开始地址
 * @param limit 段界限
 * @param base 段基址
 * @param ar 权限设置
 * @param type 0:16位，1:32位，2:64位
 */
void set_gdt_segment(struct GDT_SEG *gs, unsigned int limit, unsigned int base, char ar,int type){
	char pageset = 0;
	if(limit>0xffff){
		pageset |= 0x80;
		limit /=0x1000;
	}
	switch (type)
	{
	case 0:
		pageset &= 0xbf;
		break;
	case 1:
		pageset |= 0x40;
		break;
	case 2:
		pageset |= 0x20;
		break;
	default:
		return;
	}
	gs->limit_low = limit & 0xffff;
	gs->base_low = base & 0xffff;
	gs->base_mid = (base >> 16) & 0xff;
	gs->access_right = ar;
	gs->limit_high_pageset = pageset | ((limit>>16) & 0x0f);
	gs->base_high = (base>>24) & 0xff;
	return;
}
/**
 * @brief Set the idt segment object
 * 
 * @param ii 表项开始地址
 * @param offset 偏移地址
 * @param selector 段描述符选择子
 * @param settings 设置
 */
void set_idt_segment(struct IDT_INTGATE* ii, unsigned int offset,unsigned int selector, int settings){
	ii->offset_low = offset & 0xffff;
	ii->selector = selector & 0xffff;
	ii->settings = settings & 0xffff;
	ii->offset_high = (offset>>16)&0xffff;
	return;
}

void wait_kbc_ready(){
	while(1)
	{
		if((io_in8(0x64)&(unsigned char)0x03) == 0) return;
	}
}
void  init_keyboard(){
	wait_kbc_ready();
	io_out8(0x64,0x60);
	wait_kbc_ready();
	io_out8(0x60,0x47);
	return;
}
void enable_mouse(){
	
	wait_kbc_ready();
	io_out8(0x64,0xd4);
	wait_kbc_ready();
	io_out8(0x60,0xf4);
	return;
}
void init_PIC(){
	io_out8(PIC0_IMR,0XFF);
	io_out8(PIC1_IMR,0xff);
	io_out8(PIC0_ICW1,0x11);
	io_out8(PIC0_ICW2,0x20);
	io_out8(PIC0_ICW3,1<<2);
	io_out8(PIC0_ICW4,0x01);
	io_out8(PIC1_ICW1,0x11);
	io_out8(PIC1_ICW2,0x28);
	io_out8(PIC1_ICW3,2);
	io_out8(PIC1_ICW4,0x01);
	io_out8(PIC0_IMR,0xfb);
	io_out8(PIC1_IMR,0xff);
	return;
}
//计时器中断
void init_PIT(){
	io_out8(0x43,0x34);
	io_out8(0x40,0x9c);
	io_out8(0x40,0x2e);
	return;
}




/**
 * @brief 时钟中断程序
 * 
 * @param esp 
 */
void inthandler20h(int* esp){
	io_out8(PIC0_OCW2,0X60);
	/**
	 * @todo 时钟中断处理
	 */
	tc.time++;
	int task = 0;
	struct timer* t = (struct timer*)tc.timelist.head;
	while(1){
		if(t->timeout<=tc.time){
			int res = timer_toc();
			if(task<=2&&res>2) task = res;
			// my_sprintf(s,"20h:%d %d",res,t->timeout);
			// win_showsln(2,s,COLOR_BLACK);
			t = (struct timer*)tc.timelist.head;
		}
		else break;
	}
	if(task>2){
		change_task(task-2);
	}
	return;
}
/**
 * @brief 键盘中断处理
 * 写入缓冲区
 * @param esp 
 */
void inthandler21h(int* esp){
	io_out8(PIC0_OCW2,0x61);
	io_buffer_push(&kb_buffer_ctrl,io_in8(PORT_GETKEY));
	return;
}
/**
 * @brief 鼠标中断处理
 * 写入缓冲区
 * @param esp 
 */
void inthandler2ch(int* esp){
	io_out8(PIC1_OCW2,0x64);
	io_out8(PIC0_OCW2,0x62);
	io_buffer_push(&ms_buffer_ctrl,io_in8(PORT_GETKEY));
	return;
}
/**
 * @brief ??
 * 
 * @param esp 
 */
void inthandler27h(int* esp){
	io_out8(PIC0_OCW2,0X67);
	return;
}
void switch_task_test(){
	for(;;){
		my_sprintf(s,"b run%d",tc.time);
		win_showslr(1,s,COLOR_LIGHT_GREEN);
		hlt();
	}
}