#include"ros.h"

int mouse_create(){
	for(int i=0;i<MAX_WINDOWS;i++){
		if(windows[i].statu&0x1) continue;		
		// windows[i].name[0] = 'm';
		// windows[i].name[1] = 'o';
		// windows[i].name[2] = 's';
		windows[i].x0 = 160;
		windows[i].y0 = 100;
		windows[i].width= 8;
		windows[i].height=16;
		unsigned char* buffer = g_boxfill(8,16, 255);
		windows[i].hwnd = i;
		windows[i].sheet_index = sheet_malloc(160,100,8,16,sc->top+1,buffer,i+1);
		if(windows[i].sheet_index<0) break;
		my_strcpy(windows[i].name,"mouse",20);
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
/**
 * @brief Get the hwnd by name 
 * 
 * @param s 
 * @return int 如果有找到返回hwnd，没有返回-1.重复命名会找hwnd最小的
 */
int get_hwnd_by_name(const char* s){
	int len = my_strlen(s);
	for(int i = 0;i<MAX_WINDOWS;i++){
		if(!windows[i].statu&0x01) continue;
		if(len !=my_strlen(windows[i].name)||my_strcmp(windows[i].name,s,len)) continue;
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
		my_strcpy(windows[i].name,name,20);
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
				windows[i].height = height+2*SHADOW_PIX+WINDOWHEAD_PIX;
				buffer = g_windowfill(name,windows[i].width,windows[i].height,bg_color,windows[i].ins_width,windows[i].ins_height);
				break;
			case 2:
				windows[i].x0-=SHADOW_PIX;
				windows[i].y0-=(SHADOW_PIX+WINDOWHEAD_PIX);
				windows[i].width = width+2*SHADOW_PIX;
				windows[i].height = height+2*SHADOW_PIX+WINDOWHEAD_PIX;
				buffer = g_windowfill(name,windows[i].width,windows[i].height,bg_color,windows[i].ins_width,windows[i].ins_height);
				g_v_boxfill(buffer,windows[i].width,windows[i].width-SHADOW_PIX-22,SHADOW_PIX+3,16,16,COLOR_BLACK);
		}
		windows[i].hwnd = i;
		windows[i].sheet_index = sheet_malloc(windows[i].x0,windows[i].y0,windows[i].width,windows[i].height,z,buffer,i+1);
		int j = 0;
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
	int length = g_winshows(windows[hwnd].sheet_index,hwnd,s,windows[hwnd].cursor_x,windows[hwnd].cursor_y,font_color,windows[hwnd].width);
	sheet_refresh(windows[hwnd].sheet_index,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,length*8,16);
	windows[hwnd].cursor_y+=16;
	windows[hwnd].cursor_x=LS_INTERVAL;
	if(hwnd == focused_window->hwnd) cursor_resume();
	return;
}
/**
 * @brief 在当前行反复显示（自动回车）
 * @param hwnd 
 * @param s 
 * @param font_color 
 */
void win_showslr(int hwnd,char* s, unsigned char font_color){
	g_v_boxfill(sc->sheets[windows[hwnd].sheet_index].buf,windows[hwnd].width,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,my_strlen(s)*8,16,windows[hwnd].bg_color);
	int length = g_winshows(windows[hwnd].sheet_index,hwnd,s,windows[hwnd].cursor_x,windows[hwnd].cursor_y,font_color,windows[hwnd].width);
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
		if(windows[hwnd].cursor_y+16>=windows[hwnd].ins_height){
			g_scrollup(windows[hwnd].sheet_index,16,SHADOW_PIX,SHADOW_PIX+WINDOWHEAD_PIX,windows[hwnd].ins_width,windows[hwnd].cursor_y,windows[hwnd].width,windows[hwnd].ins_height,windows[hwnd].bg_color);
			windows[hwnd].cursor_y-=16;
		}
	}else if(c=='\b'){
		if(windows[hwnd].cursor_x==LS_INTERVAL && windows[hwnd].cursor_y==RS_INTERVAL);
		else if(windows[hwnd].cursor_x-8>=LS_INTERVAL){
			windows[hwnd].cursor_x -= 8;
			g_v_boxfill(sc->sheets[windows[hwnd].sheet_index].buf,windows[hwnd].width,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16,windows[hwnd].bg_color);
			sheet_refresh(windows[hwnd].sheet_index,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);
		}else{
			windows[hwnd].cursor_x = ((windows[hwnd].ins_width-LS_INTERVAL)/8-1)*8+LS_INTERVAL;
			windows[hwnd].cursor_y -= 16;
			g_v_boxfill(sc->sheets[windows[hwnd].sheet_index].buf,windows[hwnd].width,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16,windows[hwnd].bg_color);
			sheet_refresh(windows[hwnd].sheet_index,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);
		}
	}
	else{
		if(windows[hwnd].cursor_x+16>=windows[hwnd].ins_width){
			//超过正文内容显示区域，换行
			windows[hwnd].cursor_x = LS_INTERVAL;
			windows[hwnd].cursor_y += 16;
		}
		if(windows[hwnd].cursor_y+16>=windows[hwnd].ins_height){
			g_scrollup(windows[hwnd].sheet_index,16,SHADOW_PIX,SHADOW_PIX+WINDOWHEAD_PIX,windows[hwnd].ins_width,windows[hwnd].cursor_y,windows[hwnd].width,windows[hwnd].ins_height,windows[hwnd].bg_color);
			windows[hwnd].cursor_y-=16;
		}
		g_showc(sc->sheets[windows[hwnd].sheet_index].buf,c,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,font_color,windows[hwnd].width);
		sheet_refresh(windows[hwnd].sheet_index,windows[hwnd].cursor_x+SHADOW_PIX,windows[hwnd].cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);
		windows[hwnd].cursor_x+=8;
	}
	if(hwnd == focused_window->hwnd) cursor_resume();
}

void win_free(int hwnd){
	windows[hwnd].name[0]='\0';
	sheet_free(windows[hwnd].sheet_index);
	windows[hwnd].x0 = 0;
	windows[hwnd].y0 = 0;
	windows[hwnd].ins_width = 0;
	windows[hwnd].ins_height = 0;
	windows[hwnd].bg_color = 0;
	windows[hwnd].sheet_index = -1;
	windows[hwnd].hwnd = -1;
	windows[hwnd].statu&=0;
}