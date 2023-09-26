#include"ros.h"
//系统函数
/**
 * @brief Set the gdt segment object
 * 
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
void init_io_buffer(struct io_buffer* iobuf,int size,unsigned char* buffer){
	iobuf->p = 0;
	iobuf->q = 0;
	iobuf->flags = 0x1;
	iobuf->size = size;
	iobuf->buffer = buffer;
	return ;
}
//可能要考虑中断做成临界区？
int io_buffer_push(struct io_buffer* buffer,unsigned char data){
	cli();
	if(buffer->p==buffer->q && !(buffer->flags & 0x1)){
		buffer->flags |= 0x2;
		sti();
		return -1;
	}
	buffer->buffer[buffer->p] = data;
	buffer->p = (buffer->p+1)%buffer->size;
	if(buffer->p==buffer->q)
		buffer->flags &= 0xfe;
	sti();
	return 0;
}
//可能要考虑中断做成临界区？
int io_buffer_pop(struct io_buffer* buffer){
	cli();
	if(buffer->p==buffer->q && (buffer->flags & 0x1))
	{
		sti();
		return -1;//
	}
	unsigned char data = buffer->buffer[buffer->q];
	buffer->q = (buffer->q+1)%buffer->size;
	buffer->flags |= 0x01;
	sti();
	return (int)data;
}
int io_buffer_num(struct io_buffer* buffer){
	if(buffer->p<buffer->q)
		return buffer->p+buffer->size-buffer->q;
	else if (buffer->p==buffer->q && !(buffer->flags & 0x1)) return buffer->size;
	else return buffer->p-buffer->q;
}

void get_keyboard_input(){
	if(io_buffer_num(&kb_buffer_ctrl)) {
		int data = io_buffer_pop(&kb_buffer_ctrl);
		if(data!=-1){
			my_sprintf(s,"kb_num : %d",data);
			win_shows(1,s,10,7);
		}
	}
	return;
}

void mouse_decode(){
	mouse.btn = mouse.mouse_state[0] & 0x07;
	mouse.x = mouse.mouse_state[1];
	mouse.y = mouse.mouse_state[2];
	if(mouse.x && mouse.mouse_state[0] & 0x10)
		mouse.x |= 0xffffff00;
	if(mouse.y && mouse.mouse_state[0] & 0x20)
		mouse.y |= 0xffffff00;
	mouse.y = -mouse.y;
	//打印字符和更新鼠标，可能以后要转移到图层控制模块
	
	g_showc(1,mouse.posx,mouse.posy,COLOR_LIGHT_SKYBLUE);
	mouse.posx=(mouse.posx+mouse.x)%320;
	mouse.posy=(mouse.posy+mouse.y)%200;
	if(mouse.posx<0) mouse.posx=0;
	if(mouse.posy<0) mouse.posy=0;
	g_showc(1,mouse.posx,mouse.posy,COLOR_LIGHT_PURPLE);
}

int get_mouse_input(int ms_state){
	switch (ms_state){
		case 0:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1){
					ms_state = 1;
				}
				else break;;
			}
			else break;
		case 1:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1 && (data & 0xc8)==0x8){
					mouse.mouse_state[0]=(unsigned char)data;
					ms_state = 2;
				}
				else break;
			}
			else break;
		case 2:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1){
					mouse.mouse_state[1]=(unsigned char)data;
					ms_state = 3;
				}
				else break;
			}
			else break;
		case 3:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1){
					mouse.mouse_state[2]=(unsigned char)data;
					ms_state = 1;
					mouse_decode();
				}
				else break;
			}
			else break;
	}
	return ms_state;	
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