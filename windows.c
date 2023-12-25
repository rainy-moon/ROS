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
		windows[i].sheet_index = sheet_malloc(160,100,8,16,sc->top+1,buffer,i+1);
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
*desktop窗口为1号窗口
*debug窗口为2号窗口
*mouse为0号窗口
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
 * @param style 窗口模式
 * @return int 正常返回窗口id，错误（无法建更多窗口）-1
 */
int win_create(char* name, int x0, int y0, int width, int height,int z, unsigned char bg_color,int style){
	for(int i=0;i<MAX_WINDOWS;i++){
		if(windows[i].statu&0x1) continue;
		windows[i].x0 = x0;
		windows[i].y0 = y0;
		windows[i].ins_width = width;
		windows[i].ins_height = height;
		windows[i].bg_color=bg_color;
		char* name_temp = name;
		unsigned char* buffer;
		switch(style){
			case 0:
				windows[i].width = width;
				windows[i].height = height;
				buffer = g_boxfill(width,height, bg_color);
				break;
			case 1:
				windows[i].x0-=SHADOW_PIX;
				windows[i].y0-=(SHADOW_PIX+WINDOWHEAD_PIX);
				windows[i].width = width+2*SHADOW_PIX;
				windows[i].height = height+2*SHADOW_PIX+24;
				buffer = g_windowfill(name,windows[i].width,windows[i].height,bg_color,windows[i].ins_width,windows[i].ins_height);
				break;
			case 2:
				windows[i].x0-=SHADOW_PIX;
				windows[i].y0-=(SHADOW_PIX+WINDOWHEAD_PIX);
				windows[i].width = width+2*SHADOW_PIX;
				windows[i].height = height+2*SHADOW_PIX+24;
				buffer = g_windowfill(name,windows[i].width,windows[i].height,bg_color,windows[i].ins_width,windows[i].ins_height);
		}
		windows[i].hwnd = i;
		windows[i].sheet_index = sheet_malloc(windows[i].x0,windows[i].y0,windows[i].width,windows[i].height,z,buffer,i+1);
		int j = 0;
		for(;*name_temp;name_temp++,j++)
			windows[i].name[j] = *name;
		windows[i].name[j] = '\0';
		windows[i].statu |= (style<<8)+0x1;
		//g_shows(name,0,0,7,windows[i].sheet_index);
		windows[i].cursor_x = LS_INTERVAL;
		windows[i].cursor_y = RS_INTERVAL;
		return i;
	}
	return -1;
}
/**
 * @brief 输出一行，自动换到下一行
 * 
 * @param hwnd 
 * @param s 
 * @param font_color 
 */
void win_showsln(int hwnd, char* s, unsigned char font_color){
	if(hwnd == focused_window->hwnd) cursor_pause();
	int length = g_shows(sc->sheets[windows[hwnd].sheet_index].buf,s,windows[hwnd].cursor_x,windows[hwnd].cursor_y,font_color,windows[hwnd].width,windows[hwnd].ins_width,windows[hwnd].ins_height);
	sheet_refresh(windows[hwnd].sheet_index,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,length*8,16);
	windows[hwnd].cursor_y+=16;
	if(hwnd == focused_window->hwnd) cursor_resume();
	return;
}
/**
 * @brief 在当前行反复显示（自动回车）
 * 先默认背景色是白色，以后再进行修改
 * @param hwnd 
 * @param s 
 * @param font_color 
 */
void win_showslr(int hwnd,char* s, unsigned char font_color){
	g_v_boxfill(sc->sheets[windows[hwnd].sheet_index].buf,windows[hwnd].width,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,my_strlen(s)*8,16,windows[hwnd].bg_color);
	int length = g_shows(sc->sheets[windows[hwnd].sheet_index].buf,s,windows[hwnd].cursor_x,windows[hwnd].cursor_y,font_color,windows[hwnd].width,windows[hwnd].ins_width,windows[hwnd].ins_height);
	sheet_refresh(windows[hwnd].sheet_index,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,length*8,16);
	return;
}
/**
 * @brief 显示字符，'\n'换行
 * 
 * @param hwnd 
 * @param c 
 * @param font_color 
 */
void win_showc(int hwnd,char c,unsigned char font_color){
	if(hwnd == focused_window->hwnd) cursor_pause();
	if(c=='\n') {
		windows[hwnd].cursor_y += 16;
		windows[hwnd].cursor_x = LS_INTERVAL;
	}
	else{
		g_showc(sc->sheets[windows[hwnd].sheet_index].buf,c,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,font_color,windows[hwnd].width);
		sheet_refresh(windows[hwnd].sheet_index,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);
		windows[hwnd].cursor_x+=8;
	}
	if(hwnd == focused_window->hwnd) cursor_resume();
}

