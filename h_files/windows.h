#ifndef _WINDOWSH_
#define _WINDOWSH_
#include "ros.h"
#define MAX_WINDOWS			10
#define SHADOW_PIX			6		//窗口边缘羽化度
#define WINDOWHEAD_PIX		24		//类型1窗口名高度
#define LS_INTERVAL			8		//行距正文左侧开始间隔
#define RS_INTERVAL			4		//列距正文上方开始间隔
// struct define
struct window
{
	//窗体名字不超过19字符
	char name[20];
	int hwnd;
	int cursor_x; int cursor_y;//光标位置(相对窗口正文可输入开始位置)
	int x0; int y0;
	int width; int height;
	int ins_width;int ins_height;
	unsigned char bg_color;
	int statu;
	/*
	* 0bit 1已使用 0未使用
	* 1-3bit 状态位 000激活
	* 4-7bit 窗口样式	0无边框(桌面鼠标等，无法进行输入输出重定向) 
	*				  	1标准窗口（允许输入输出重定向）
	*/
	int sheet_index;//绑定图层
}windows[MAX_WINDOWS];

struct MOUSE{
	char mouse_state[3];
	int hwnd;//窗口id
	int x,y;//鼠标移动数据
	int ms_state,btn; 
	int posx,posy; //鼠标位置
}mouse;
// global define

//当前输入定向窗口
struct window* focused_window;

// function define
int mouse_create();
int win_create(char* name, int x0, int y0, int width, int height,int z, unsigned char bg_color, int style);
void win_showsln(int hwnd, char* s, unsigned char font_color);
void win_showslr(int hwnd, char* s, unsigned char font_color);
void win_showc(int hwnd,char c,unsigned char font_color);
#endif