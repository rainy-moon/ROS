#include"ros.h"

int mouse_create(){
	for(int i=0;i<MAX_WINDOWS;i++){
		if(windows[i].statu&0x1) continue;
		windows[i].x0 = 160;
		windows[i].y0 = 100;
		windows[i].width= 8;
		windows[i].height=16;
		unsigned char* buffer = g_boxfill(8,16, 255);
		windows[i].hwnd = i;
		windows[i].sheet_index = sheet_malloc(160,100,8,16,sc->top+1,buffer);
		if(windows[i].sheet_index<0) break;
		windows[i].statu |= 0x1;
		g_showc(buffer,1,0,0,COLOR_LIGHT_PURPLE,windows[i].width);
		sheet_refresh(windows[i].sheet_index,0,0,8,16);
		windows[i].cursor_x = 0;
		windows[i].cursor_y = 0;
		mouse.hwnd = i;
		return i;
	}
	return -1;
}

/*默认窗口
*desktop窗口为0号窗口
*debug窗口为1号窗口
*/

/**
 * @brief 创建窗口，关联一个图层
 * 
 * @param name 
 * @param x0 
 * @param y0 
 * @param width 
 * @param height 
 * @param z 创建时窗口z层数
 * @param bg_color 
 * @return int 正常返回窗口id，错误（无法建更多窗口）-1
 */
int win_create(char* name, int x0, int y0, int width, int height,int z, unsigned char bg_color,int style){
	for(int i=0;i<MAX_WINDOWS;i++){
		if(windows[i].statu&0x1) continue;
		windows[i].x0 = x0;
		windows[i].y0 = y0;
		windows[i].width= width;
		windows[i].height=height;
		char* name_temp = name;
		unsigned char* buffer;
		switch(style){
			case 0:
				buffer = g_boxfill(windows[i].width,windows[i].height, bg_color);
				break;
			case 1:
				windows[i].x0-=6;
				windows[i].y0-=6;
				windows[i].width+=12;
				windows[i].height+=36;
				buffer = g_windowfill(name,windows[i].width,windows[i].height,bg_color);
				break;
		}
		windows[i].hwnd = i;
		windows[i].sheet_index = sheet_malloc(windows[i].x0,windows[i].y0,windows[i].width,windows[i].height,z,buffer);
		int j = 0;
		for(;*name_temp;name_temp++,j++)
			windows[i].name[j] = *name;
		windows[i].name[j] = '\0';
		windows[i].statu |= (style<<4)+0x1;
		//g_shows(name,0,0,7,windows[i].sheet_index);
		windows[i].cursor_x = 6;
		windows[i].cursor_y = 24;
		return i;
	}
	return -1;
}

void win_shows(int hwnd, char* s, unsigned char font_color){
	/**
	 * @brief 在窗口内打印字符串，该函数暂时是自动换行
	 * @param int hwnd, char* s, int length, unsigned char font_color
	 */
	int length = g_shows(sc->sheets[windows[hwnd].sheet_index].buf,s,windows[hwnd].cursor_x,windows[hwnd].cursor_y,font_color,windows[hwnd].width);
	sheet_refresh(windows[hwnd].sheet_index,windows[hwnd].cursor_x,windows[hwnd].cursor_y,length*8,16);
	windows[hwnd].cursor_y+=16;
	return;
}