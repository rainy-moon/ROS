#ifndef _GRAPHAFONTH_
#define _GRAPHAFONTH_
#include "ros.h"
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
#define MAX_SHEETS			32 		//最大图层数


//struct define
struct SHEET{
	int z,width,height;
	int x,y;
	int flag; //标记是否使用  0未使用 使用则绑定窗口hwnd+1,注意+1！！！
	unsigned char* buf;
};
struct sheet_ctrl{
	int maxx,maxy,top,resv;//resv保留位
	struct SHEET *sheet_zlevel[MAX_SHEETS];
	struct SHEET sheets[MAX_SHEETS];
};

//global define
struct sheet_ctrl* sc;				//图层控制器
unsigned char* screen;				//画面渲染地址（0x40000000）
struct SHEET* *screen_buf;			//最上图层记录

//function define
void init_color_plate();
void g_v_boxfill(unsigned char*p,int sizex, int x0,int y0,int width,int height,unsigned char color);
unsigned char* g_boxfill(int width, int height, unsigned char color);
void g_v_edgefill(unsigned char*p,int sizex, int x0,int y0,int board,int width,int height,unsigned char color);
unsigned char* g_edgefill(int board, int width,int height,unsigned char color);
void g_showc(unsigned char*p, char c, int x0, int y0, char color ,int sizex);
int g_winshows(int sheet_index,int hwnd, const char* string, int x0, int y0, char color,int sizex);
void g_shows(unsigned char* p,const char* string, int x0, int y0, char color,int sizex);
void g_scrollup(int sheet_index,int pixes,int x0, int y0, int width, int height,int sizex,int ins_y,unsigned char bg_color);
void init_sheet_ctrl();
void sheet_updatez(int index,int z);
void sheet_hide(int index);
void sheet_display(int index,int z);
int sheet_malloc(int x,int y,int width,int height,int z,unsigned char* buffer,int hwndp1);
void sheet_free(int index);
void init_screen_buf();
void sheet_refresh(int index,int x0,int y0,int width,int height);
int pix_in_sheet(int x,int y,int z);
void sheet_slide(int x,int y,int index);
#endif