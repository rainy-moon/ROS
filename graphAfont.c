#include"ros.h"

void init_color_plate(){
	unsigned char color_rgb[16][3] = {
		{0x00,0x00,0x00}, 	//0：黑色
		{0xff,0x00,0x00},	//1：亮红
		{0x00,0xff,0x00},	//2：亮绿
		{0x00,0x00,0xff},	//3：亮蓝
		{0xff,0xff,0x00},	//4：亮黄
		{0xff,0x00,0xff},	//5：亮紫
		{0x00,0xff,0xff},	//6：浅亮蓝
		{0xff,0xff,0xff},	//7：白色
		{0xc6,0xc6,0xc6},	//8：亮灰
		{0x84,0x00,0x00},	//9：暗红
		{0x00,0x84,0x00},	//10：暗绿
		{0x00,0x00,0x84},	//11：暗蓝（青）
		{0x84,0x84,0x00},	//12：暗黄
		{0x84,0x00,0x84},	//13：暗紫
		{0x00,0x84,0x84},	//14: 浅暗蓝
		{0x84,0x84,0x84}	//15: 暗灰
	};
	cli();
	io_out8(0x03c8,0);
	for(int i=0;i<16;i++){
		io_out8(0x03c9,color_rgb[i][0]/4);
		io_out8(0x03c9,color_rgb[i][1]/4);
		io_out8(0x03c9,color_rgb[i][2]/4);
	}
	sti();
	return;
}
/**
 * @brief 给出填充矩形的像素数组
 * 
 * @param width
 * @param height 
 * @param color 
 * @return unsigned* 
 */
unsigned char* g_boxfill(int width, int height, unsigned char color){
	
	unsigned char* p = (unsigned char*)mem_malloc(width*height);
	for(int i=0;i<width*height;i++) p[i] = color; 
	return p;
}
/**
 * @brief 给出填充边框的像素数组
 * 
 * @param board 
 * @param width 
 * @param height 
 * @param color 
 * @return unsigned* 
 */
unsigned char* g_edgefill(int board, int width,int height,unsigned char color){
	
	unsigned char* p =(unsigned char*)mem_malloc(width*height);
	int i = 0, mid = height-2*board;
	for(;i<board*width;i++)
		p[i] = color;
	for(;i<mid*width;i++){
		if(i%width<board || i%width>=width-board) p[i] = color;
		else p[i] = COLOR_TRANSP;
	}
	for(;i<width*height;i++)
		p[i] = color;
	return p;
}
/**
 * @brief 在某一图层的内容数组buf上的相对位置加上一个字符
 * 
 * @param c 字符
 * @param x0 相对位置x
 * @param y0 相对位置y
 * @param color 
 * @param index 想要绘制的图层
 */
void g_showc(char c, int x0, int y0, char color ,int index){
	
	extern char WORDS[128][8];
	unsigned char* p = sc->sheets[index].buf;
	for(int y = y0;y < y0+8;y++){
		for(int x = x0;x < x0+8;x++){
			if(WORDS[c][y-y0] & (0x80>>(x-x0))) p[y*sc->sheets[index].width+x] = color;
		}
	}
	return;
}
/**
 * @brief 在某一图层的内容数组buf上的相对位置加上一个字符串
 * 
 * @param string 
 * @param x0 相对位置x
 * @param y0 相对位置y
 * @param color 
 * @param index 想要绘制的图层
 */
void g_shows(const char* string, int x0, int y0, char color,int index){
	
	extern char WORDS[128][8];
	int x = x0;
	for(;*string;string++,x+=8){
		g_showc(*string,x,y0,color,index);
	}
	sheet_refresh(index,x0,y0,x-x0,8);
	return;
}

void init_sheet_ctrl(){
	sc = (struct sheet_ctrl*)mem_malloc(sizeof(struct sheet_ctrl));
	sc->maxx = 320;sc->maxy=200;
	sc->top = -1;
	for(int i=0;i<MAX_SHEETS;i++){
		sc->sheets[i].flag=0;
		sc->sheets[i].z = -1;
		sc->sheets[i].x = 0;
		sc->sheets[i].y = 0;
		sc->sheets[i].height = 0;
		sc->sheets[i].width = 0;
		sc->sheets[i].buf = (unsigned char*)NULL;
	}
	return;
}
/**
 * @brief 更新高度,原图层没有隐藏，更新后也不隐藏
 * 
 * @param index 
 * @param z 
 */
void sheet_updatez(int index,int z){
	int old = sc->sheets[index].z;
	if(old == z) return; 
	else if(old>z){//下调
		struct SHEET* temp = sc->sheet_zlevel[old];
		for(int i=old;i>z;i--){
			sc->sheet_zlevel[i] = sc->sheet_zlevel[i-1];
			sc->sheet_zlevel[i]->z ++;
		}
		sc->sheet_zlevel[z] = temp;
		sc->sheet_zlevel[z]->z = z;
		int x1 =  sc->sheets[index].x+sc->sheets[index].width;
		int y1 = sc->sheets[index].y+sc->sheets[index].height;
		for(int i = sc->sheets[index].y;i<y1;i++)
			for(int j=sc->sheets[index].x;j<x1;j++){
				if(screen_buf[i][j]!=&(sc->sheets[index])) continue;
				else{
					for(int k = old;k>=z;k--)
						if(pix_in_sheet(j,i,k)){
							screen_buf[i][j] = sc->sheet_zlevel[k];
							if(sc->sheet_zlevel[k]->buf[(i-sc->sheet_zlevel[k]->y)*sc->sheet_zlevel[k]->width+(j-sc->sheet_zlevel[k]->x)]!=COLOR_TRANSP)screen[i*320+j] = sc->sheet_zlevel[k]->buf[(i-sc->sheet_zlevel[k]->y)*sc->sheet_zlevel[k]->width+(j-sc->sheet_zlevel[k]->x)];
							break;
						}
				}
			}
		return;
	}
	else{//上调
		struct SHEET* temp = sc->sheet_zlevel[old];
		for(int i=old;i<z;i++){
			sc->sheet_zlevel[i] = sc->sheet_zlevel[i+1];
			sc->sheet_zlevel[i]->z--;
		}
		sc->sheet_zlevel[z] = temp;
		sc->sheet_zlevel[z]->z = z;
		int x1 =  sc->sheets[index].x+sc->sheets[index].width;
		int y1 = sc->sheets[index].y+sc->sheets[index].height;
		for(int i = sc->sheets[index].y;i<y1;i++)
			for(int j=sc->sheets[index].x;j<x1;j++){
				if(screen_buf[i][j]==&(sc->sheets[index])) continue;
				else if(screen_buf[i][j]->z<z) {
					screen_buf[i][j] = sc->sheet_zlevel[z];
					if(sc->sheet_zlevel[z]->buf[(i-sc->sheets[index].y)*sc->sheets[index].width+(j-sc->sheets[index].x)]!=COLOR_TRANSP)screen[i*320+j] = sc->sheet_zlevel[z]->buf[(i-sc->sheets[index].y)*sc->sheets[index].width+(j-sc->sheets[index].x)];
				}
			}
		return;
	}
}
void sheet_hide(int index){
	if(sc->sheets[index].z<0);
	else{
		for(int i = sc->sheets[index].z;i<sc->top;i++){
			sc->sheet_zlevel[i] = sc->sheet_zlevel[i+1];
			sc->sheet_zlevel[i]->z--;
		}
		sc->sheet_zlevel[sc->top] = (struct SHEET*)NULL;
		sc->top--;
		//update screen_buf:
		int x1 =  sc->sheets[index].x+sc->sheets[index].width;
		int y1 = sc->sheets[index].y+sc->sheets[index].height;
		for(int i = sc->sheets[index].y;i<y1;i++)
			for(int j=sc->sheets[index].x;j<x1;j++){
				if(screen_buf[i][j]!=&(sc->sheets[index])) continue;
				else{
					for(int k = sc->sheets[index].z-1;k>=0;k--)
						if(pix_in_sheet(j,i,k)){
							screen_buf[i][j] = sc->sheet_zlevel[k];
							if(sc->sheet_zlevel[k]->buf[(i-sc->sheet_zlevel[k]->y)*sc->sheet_zlevel[k]->width+(j-sc->sheet_zlevel[k]->x)]!=COLOR_TRANSP)screen[i*320+j] = sc->sheet_zlevel[k]->buf[(i-sc->sheet_zlevel[k]->y)*sc->sheet_zlevel[k]->width+(j-sc->sheet_zlevel[k]->x)];
							break;
						}
				}
			}
		sc->sheets[index].z = -1;
	}
	return;
}
void sheet_display(int index,int z){
	if(sc->sheets[index].z>=0);
	else{
		for(int i=sc->top+1;i>z;i--){
			sc->sheet_zlevel[i]=sc->sheet_zlevel[i-1];
			sc->sheet_zlevel[i]->z--;
		}
		sc->sheet_zlevel[z] = &(sc->sheets[index]);
		sc->top ++;
		sc->sheets[index].z = z;
		int x1 =  sc->sheets[index].x+sc->sheets[index].width;
		int y1 = sc->sheets[index].y+sc->sheets[index].height;
		for(int i = sc->sheets[index].y;i<y1;i++)
			for(int j=sc->sheets[index].x;j<x1;j++){
				if(screen_buf[i][j]->z<z) {
					screen_buf[i][j] = sc->sheet_zlevel[z];
					if(sc->sheet_zlevel[z]->buf[(i-sc->sheets[index].y)*sc->sheets[index].width+(j-sc->sheets[index].x)]!=COLOR_TRANSP)screen[i*320+j] = sc->sheet_zlevel[z]->buf[(i-sc->sheets[index].y)*sc->sheets[index].width+(j-sc->sheets[index].x)];
				}
			}
	}
	return;
}
/**
 * @brief 注册一个图层，返回其所在的sheets数组下标，-1失败
 * 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @param z 
 * @param buffer 
 * @return int 
 */
int sheet_malloc(int x,int y,int width,int height,int z,unsigned char* buffer){
	for(int i=0;i<MAX_SHEETS;i++){
		if(sc->sheets[i].flag==0){
			sc->sheets[i].flag=1;
			sc->sheets[i].x=x;
			sc->sheets[i].y=y;
			sc->sheets[i].width=width;
			sc->sheets[i].height=height;
			sc->sheets[i].buf=buffer;
			sheet_display(i,z);
			return i;
		}
	}
	return -1;
}

void sheet_free(int index){
	sheet_hide(index);
	mem_free(sc->sheets[index].buf,sc->sheets[index].width*sc->sheets[index].height);
	sc->sheets[index].flag=0;
	return;
}
void init_screen_buf(){
	screen_buf = (struct SHEET ***)mem_malloc(sizeof(unsigned int[200][320]));
	return;
}
/**
 * @brief 更新图层（针对内容更新和移动）
 * 
 * @param index 
 * @param x0 相对坐标
 * @param y0 相对坐标
 * @param width 
 * @param height 
 */
void sheet_refresh(int index,int x0,int y0,int width,int height){
	int x1 = x0+width;
	int y1 = y0+height;
	for(int i = y0;i<y1;i++)
		for(int j=x0;j<x1;j++){
			if(screen_buf[i][j] == &(sc->sheets[index])) 
				if(sc->sheets[index].buf[i*sc->sheets[index].width+j] != COLOR_TRANSP) screen[(i+sc->sheets[index].y)*320+j+sc->sheets[index].x] = sc->sheets[index].buf[i*sc->sheets[index].width+j];
		}
}
/**
 * @brief 判断像素点是否在z层图层中，在返回1，否0
 * 
 * @param x 
 * @param y 
 * @param z 
 * @return int 
 */
int pix_in_sheet(int x,int y,int z){
	if(x>=sc->sheet_zlevel[z]->x && x<sc->sheet_zlevel[z]->x+sc->sheet_zlevel[z]->width
	&& y>=sc->sheet_zlevel[z]->y && y<sc->sheet_zlevel[z]->y+sc->sheet_zlevel[z]->height)
		return 1;
	else return 0;
}

void sheet_slide(int x,int y,int index){
	int h = sc->sheets[index].z;
	sheet_hide(index);
	sc->sheets[index].x=x;
	sc->sheets[index].y=y;
	sheet_display(index,h);
	return;
}