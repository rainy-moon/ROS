//停机指令
#ifndef _ROSH_
#define _ROSH_
#define COLOR_BLACK 		0
#define CLOLR_LIGHT_RED 	1
#define COLOR_LIGHT_GREEN 	2
#define COLOR_LIGHT_BLUE   	3
#define COLOR_LIGHT_YELLOW	4
#define COLOR_LIGHT_PURPLE	5
#define COLOR_LIGHT_SKYBLUE	6
#define COLOR_WHITE			7
#define COLOR_LIGHT_GREY	8
#define COLOR_DARK_RED		9
#define COLOR_DARK_GREEN	10
#define COLOR_DARK_BLUE		11
#define COLOR_DARK_YELLOW	12
#define COLOR_DARK_PURPLE	13
#define COLOR_DARK_SKYBLUE	14
#define COLOR_DARK_GREY		15
#define COLOR_TRANSP		255
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
#define MAX_SHEETS		32
#define MAX_WINDOWS		10
#define NULL  			0
struct window
{
	//窗体名字不超过19字符
	char name[20];
	int hwnd;
	int cursor_x; int cursor_y;//光标位置
	int x0; int y0;
	int width; int height;
	int statu;
	int sheet_index;//绑定图层
}windows[MAX_WINDOWS];
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
/**
 * @brief io缓冲区
 * flags 0x1 空置位 0x2 溢出位
 * 		 0满，1有空位 1溢出
 */
struct io_buffer{
	unsigned char* buffer;
	int p,q,size;
	char flags;
};
struct MOUSE{
	char mouse_state[3];
	int x,y,ms_state,btn;
	int posx,posy;
}mouse;
struct SHEET{
	int z,width,height;
	int x,y;
	int flag; //标记是否使用 1使用 0未使用
	unsigned char* buf;
};
struct sheet_ctrl{
	int top,maxx,maxy;
	struct SHEET *sheet_zlevel[MAX_SHEETS];
	struct SHEET sheets[MAX_SHEETS];
};

struct io_buffer kb_buffer_ctrl;
struct io_buffer ms_buffer_ctrl;
struct sheet_ctrl* sc;
unsigned char kb_buffer[64];
unsigned char ms_buffer[128];
unsigned char s[20];
unsigned char* screen = 0xa0000;
struct SHEET* ** screen_buf;
//sysfunc.asm 汇编定义函数，寄存器级操作
void hlt();
void sti();
void cli();
void io_out8(int port,int data);
void io_out16(int port,int data);
void io_out32(int port,int data);
unsigned char io_in8(int port);
unsigned short io_in16(int port);
unsigned int io_in32(int port);
void load_gdt(int limit,int addr);
void load_idt(int limit,int addr);
void asm_inthandler21h();
void asm_inthandler27h();
void asm_inthandler2ch();
void stihlt();
//graphAfont.c 图像画面操作函数
void init_color_plate();
unsigned char* g_boxfill(int width, int height, unsigned char color);
unsigned char* g_edgefill(int board, int width,int height,unsigned char color);
void g_showc(char c, int x0, int y0, char color ,int index);
void g_shows(const char* string, int x0, int y0, char color,int index);
void init_sheet_ctrl();
void sheet_updatez(int index,int z);
void sheet_hide(int index);
void sheet_display(int index,int z);
int sheet_malloc(int x,int y,int width,int height,int z,unsigned char* buffer);
void sheet_free(int index);
void init_screen_buf();
void sheet_refresh(int index,int x0,int y0,int width,int height);
int pix_in_sheet(int x,int y,int z);
void sheet_slide(int x,int y,int index);
//窗口package windows.c
int win_create(char* name, int x0, int y0, int width, int height,int z, unsigned char bg_color);
void win_shows(int hwnd, char* s, unsigned char font_color);

//部分c库简单复现 libc.c
/**
 * @brief ﹢整形转换字符串，注意s最好初始化15个字符
 * 
 * @param s 字符串预留内存位置
 * @param data 正整形
 */
void my_itos(char*s, int data);
/**
 * @brief 类似sprintf(s,"... %d",...)目前支持一个带替换参数，类型%d
 * 注意s初始化内存个数，避免溢出
 * @param s 
 * @param ss 
 * @param data 
 */
void my_sprintf(char* s,const char* ss,...);

//system.c系统设置函数

void set_gdt_segment(struct GDT_SEG *gs, unsigned int limit, unsigned int base, char ar,int type);
void set_idt_segment(struct IDT_INTGATE* ii, unsigned int offset,unsigned int selector, int settings);
void init_PIC();
void init_io_buffer(struct io_buffer* iobuf,int size,unsigned char* buffer);
int io_buffer_push(struct io_buffer* buffer,unsigned char data);
int io_buffer_pop(struct io_buffer* buffer);
int io_buffer_num(struct io_buffer* buffer);
void init_keyboard();
void enable_mouse();
void get_keyboard_input();
int get_mouse_input(int ms_state);

//memory.c 系统内存分配函数
int memcheck(int addrs,int addre);
void update_page_ctrl(int size,int start,int mode);
void* mem_malloc(int size);
void init_page_ctrl();
int mem_free(void* addr,int size);
#endif