#include"ros.h"

//屏幕 sc->maxxxsc->maxy
//存放软盘2e0处
void MAIN(){
	//重新初始化全局描述符表和中断描述符表

	for(int i = 0;i<8192;i++)
		set_gdt_segment(gs+i,0,0,0,0);
	for(int i = 0;i<256;i++)
		set_idt_segment(ii+i,0,0,0);
	set_gdt_segment(gs+1,0xffffffff,0,(char)0x92,1);
	set_gdt_segment(gs+2,0x000fffff,0x00101000,(char)0x9a,1);

	set_idt_segment(ii+0x20,(int)asm_inthandler20h-0x101000,2<<3,AR_INTGATE32);
	set_idt_segment(ii+0x21,(int)asm_inthandler21h-0x101000,2<<3,AR_INTGATE32);
	set_idt_segment(ii+0x27,(int)asm_inthandler27h-0x101000,2<<3,AR_INTGATE32);
	set_idt_segment(ii+0x2c,(int)asm_inthandler2ch-0x101000,2<<3,AR_INTGATE32);
	load_gdt(0xffff,0x00500000);
	load_idt(0x07ff,0x00510000);
	init_PIC();
	init_PIT();
	//初始化io缓冲区
	init_io_buffer(&kb_buffer_ctrl,64,kb_buffer);
	init_io_buffer(&ms_buffer_ctrl,128,ms_buffer);
	init_io_buffer(&tm_buffer_ctrl,128,tm_buffer);
	//初始化鼠标
	init_keyboard();
	enable_mouse();
	
	//开中断
	io_out8(PIC0_IMR, 0xf8); 
	io_out8(PIC1_IMR, 0xef);
	
	//初始化页表
	init_page_ctrl();
	//初始化调色板
	init_color_plate();
	//初始化图层
	init_sheet_ctrl();
	//初始化屏幕图层记录map
	unsigned char* *tp = (unsigned char**)0xffc;
	screen = *tp;
	init_screen_buf();
	//显示鼠标
	mouse.x=0;mouse.y=0;
	mouse.ms_state=0;
	mouse.posx = 160;mouse.posy=100;
	mouse_create();
	//桌面图层
	win_create("ROS_desktop",0,0,sc->maxx,sc->maxy,sc->top,6,0);
	//调试窗口
	win_create("debug",200,100,500,400,sc->top,0,1);
	//初始化计时器
	init_timerctrl();
	//初始化多任务控制器
	init_multipc_ctrl();

	//
	
	int pid = create_task((int)&switch_task_test,0,0);
	if(pid) regtask(pid,103,AR_TSS32);
	//加入10s计时器
	timer_malloc(1000,0,34);

	//打开中断
	sti();
	while(1) {
		time_count++;
		get_timer_input();
		get_keyboard_input();
		mouse.ms_state = get_mouse_input(mouse.ms_state);
	}
	return;
}
#include"system.c"
#include"graphAfont.c"
#include"windows.c"
#include"libc.c"
#include"memory.c"
#include"simlist.c"
#include"timer.c"
#include"iobuffer.c"
#include"multitask.c"
#include"get_keyboard_input.c"
#include"get_mouse_input.c"
#include"get_timer_input.c"