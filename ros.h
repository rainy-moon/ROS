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
#define AR_TSS32		0x0089
#define MAX_SHEETS		32
#define MAX_WINDOWS		10
#define MAX_TIMER		256
#define MAX_PROCESS		32
#define SHADOW_PIX		6
#define SWITCH_TASK_INTERVEL 	100
#define SWITCH_TASK_DARA 		1
#define NULL  			0
//多任务状态定义
#define RUNNING			1
#define RUNABLE			2
struct window
{
	//窗体名字不超过19字符
	char name[20];
	int hwnd;
	int cursor_x; int cursor_y;//光标位置
	int x0; int y0;
	int width; int height;
	int statu;
	/*
	* 0bit 1已使用 0未使用
	* 1-3bit 状态位 000激活
	* 4-7bit 窗口样式 0无边框(桌面鼠标等) 1标准窗口
	*/
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
	int hwnd;//窗口id
	int x,y;//鼠标移动数据
	int ms_state,btn; 
	int posx,posy; //鼠标位置
}mouse;

struct SHEET{
	int z,width,height;
	int x,y;
	int flag; //标记是否使用 1使用 0未使用
	unsigned char* buf;
};
struct sheet_ctrl{
	int maxx,maxy,top,resv;//resv保留位
	struct SHEET *sheet_zlevel[MAX_SHEETS];
	struct SHEET sheets[MAX_SHEETS];
};

struct TSS{
	int backlink;
	int esp0,ss0,esp1,ss1,esp2,ss2;
	int gr3;
	int eip,eflags;
	int eax,ecx,edx,ebx,esp,ebp,esi,edi;
	int es,cs,ss,ds,fs,gs;
	int ldtr;
	int iomap;
};
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
void load_tr(int tr);
void taskchange(int eip,int cs);//farjump
void asm_inthandler20h();
void asm_inthandler21h();
void asm_inthandler27h();
void asm_inthandler2ch();
void stihlt();

//graphAfont.c 图像画面操作函数

void init_color_plate();
void g_v_boxfill(unsigned char*p,int sizex, int x0,int y0,int width,int height,unsigned char color);
unsigned char* g_boxfill(int width, int height, unsigned char color);
void g_v_edgefill(unsigned char*p,int sizex, int x0,int y0,int board,int width,int height,unsigned char color);
unsigned char* g_edgefill(int board, int width,int height,unsigned char color);
void g_showc(unsigned char*p, char c, int x0, int y0, char color ,int sizex);
int g_shows(unsigned char*p,const char* string, int x0, int y0, char color,int sizex);
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

int mouse_create();
int win_create(char* name, int x0, int y0, int width, int height,int z, unsigned char bg_color, int style);
void win_showsln(int hwnd, char* s, unsigned char font_color);
void win_showslr(int hwnd, char* s, unsigned char font_color);

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
void init_PIT();
void init_io_buffer(struct io_buffer* iobuf,int size,unsigned char* buffer);
int io_buffer_push(struct io_buffer* buffer,unsigned char data);
int io_buffer_pop(struct io_buffer* buffer);
int io_buffer_num(struct io_buffer* buffer);
void init_keyboard();
void enable_mouse();
void get_timer_input();
void get_keyboard_input();
int get_mouse_input(int ms_state);
void switch_task_test();
//memory.c 系统内存分配函数
int memcheck(int addrs,int addre);
void update_page_ctrl(int size,int start,int mode);
void* mem_malloc(int size);
void init_page_ctrl();
int mem_free(void* addr,int size);

//simlist.c 模拟链表函数
/**
 * @brief 模拟链表
 * @param size 链表能使用总长度
 * @param length 当前链表长度
 */
struct SIMLIST{
	struct node* head;
	int size;
};
struct node{
	int val[255];
	struct node* next;
};
void init_simlist(struct SIMLIST* sl);
int simlist_insert(struct SIMLIST* sl,int index,struct node*new);
int simlist_delete(struct SIMLIST* sl,int index);
struct node* simlist_get(struct SIMLIST* sl,int index);
int simlist_free(struct SIMLIST* sl);
void simlist_sortedinsert(struct SIMLIST* sl,struct node* new,int index);

//timerAmultitask.c 计时器和多任务
struct timer_ctrl{
	struct SIMLIST timelist;
	int time;
	int num;
}tc;
/**
 * @brief 计时器
 * @param timeout 超时时的当前计数（time+tc.time）
 * @param time 计时长度
 * @param data 超时时向缓冲区内写的内容
 * @param flags 状态标志位
 * 0bit 0循环任务 1单次任务
 */
struct timer{
	unsigned int timeout;
	unsigned int time;
	int flags;
	int tid;
	int data;
	int reserve[251];
	struct timer* next;
};
int init_timerctrl();
int timer_malloc(unsigned int timeout,int flags,int data);
int timer_delete(int tid);
int timer_reset(int tid,unsigned int timeout,int flags,int data);
int timer_toc();

struct prograss{
	int statu;
	int flags;
	int parent;
	int pid;
	int level;
	struct TSS tss;
	int reserve[225];
}prograsses[MAX_PROCESS];
struct prograss_ctrl{
	int stt_tid;			//switch task timer tid
	struct prograss *PC; 	//当前进行进程
	struct SIMLIST PR;		//就绪进程链表
	struct SIMLIST PS;		//中断挂起链表
	struct SIMLIST PD;		//不可中断挂起链表
	struct SIMLIST PZ;		//僵尸进程链表
	struct SIMLIST PN;		//创建或等待就绪链表
}multipc;
//全局量
int time_count = 0;
struct io_buffer kb_buffer_ctrl;
struct io_buffer ms_buffer_ctrl;
struct io_buffer tm_buffer_ctrl;
struct sheet_ctrl* sc;
unsigned char kb_buffer[64];
unsigned char ms_buffer[128];
unsigned char tm_buffer[128];
unsigned char s[20];
unsigned char* screen;
struct SHEET* *screen_buf;
void init_TSS(struct TSS* tss);
int regtask(struct GDT_SEG*gs,struct TSS* base,unsigned int limit,unsigned int settings,int funcaddr);
#endif