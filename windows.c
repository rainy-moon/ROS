#include"ros.h"

int mouse_create(){
	for(int i=0;i<MAX_WINDOWS;i++){
		if(windows[i].statu) continue;
		windows[i].x0 = 160;
		windows[i].y0 = 100;
		windows[i].width= 8;
		windows[i].height=8;
		unsigned char* buffer = g_boxfill(8,8, 255);
		windows[i].hwnd = i;
		windows[i].sheet_index = sheet_malloc(160,100,8,8,sc->top+1,buffer);
		if(windows[i].sheet_index<0) break;
		windows[i].statu = 1;
		g_showc(1,0,0,COLOR_LIGHT_PURPLE,sc->top);
		sheet_refresh(windows[i].sheet_index,0,0,8,8);
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
int win_create(char* name, int x0, int y0, int width, int height,int z, unsigned char bg_color){
	for(int i=0;i<MAX_WINDOWS;i++){
		if(windows[i].statu) continue;
		windows[i].x0 = x0;
		windows[i].y0 = y0;
		windows[i].width= width;
		windows[i].height=height;
		char* name_temp = name;
		unsigned char* buffer = g_boxfill(width,height, bg_color);
		windows[i].hwnd = i;
		windows[i].sheet_index = sheet_malloc(x0,y0,width,height,z,buffer);
		int j = 0;
		for(;*name_temp;name_temp++,j++)
			windows[i].name[j] = *name;
		windows[i].name[j] = '\0';
		windows[i].statu = 1;
		g_shows(name,0,0,7,windows[i].sheet_index);
		windows[i].cursor_x = 0;
		windows[i].cursor_y = 8;
		return i;
	}
	return -1;
}

void win_shows(int hwnd, char* s, unsigned char font_color){
	/**
	 * @brief 在窗口内打印字符串，该函数暂时是自动换行
	 * @param int hwnd, char* s, int length, unsigned char font_color
	 */
	g_shows(s,windows[hwnd].cursor_x,windows[hwnd].cursor_y,font_color,windows[hwnd].sheet_index);
	windows[hwnd].cursor_y+=8;
	return;
}