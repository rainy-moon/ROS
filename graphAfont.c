#include"ros.h"

void init_color_plate(){
	unsigned char color_rgb[22][3] = {
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
		{0x84,0x84,0x84},	//15: 暗灰
		{246,246,246},		//16: 阴影用色1
		{242,242,242},
		{238,238,238},
		{230,230,230},
		{222,222,222},
		{212,212,212},		//21: 阴影用色6
	};
	cli();
	io_out8(0x03c8,0);
	for(int i=0;i<22;i++){
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
 * @brief 不新建数组，填充矩形，需要手动刷新
 * 
 * @param p 数组
 * @param sizex 该数组矩形区域宽度
 * @param x0 相对位置坐标
 * @param y0 相对位置坐标
 * @param width 填充宽度
 * @param height 填充高度
 * @param color 
 */
void g_v_boxfill(unsigned char*p,int sizex, int x0,int y0,int width,int height,unsigned char color){
	int start = y0*sizex+x0;
	for(int i=0;i<height;i++){
		for(int j = start;j<start+width;j++){
			p[j]=color;
		}
		start+=sizex;
	}
	return;
}
/**
 * @brief 不新建数组，在已有数组上加边框，需要手动刷新
 * 
 * @param p 数组
 * @param sizex 原数组矩形区域宽度
 * @param x0 起始左上角相对x坐标
 * @param y0 起始左上角相对y坐标
 * @param board 
 * @param width 边框长度
 * @param height 边框宽度
 * @param color 
 */
void g_v_edgefill(unsigned char*p,int sizex, int x0,int y0,int board,int width,int height,unsigned char color){
	int i = x0*sizex+y0, mid = height-2*board;
	int step1 = sizex-width;
	int step2 = sizex-board;
	for(int j = 1;j<=board;j++){
		for(int k=0;k<width;k++,i++){
			p[i]=color;
			p[i+(height-board)*sizex]=color;
		}
		i += step1;
	}
	for(int j=1;j<=mid;j++){
		for(int k=0;k<board;k++,i++){
			p[i]=color;
			p[i+width-board]=color;
		}
		i += step2;
	}
	return;
}
/**
 * @brief 绘制标准窗口，包括标题，按钮，阴影，样式码：1
 * 
 * @param name 窗口标题
 * @param width 宽度（如果超过窗口限制会自动调整满屏）
 * @param height 高度（如果超过窗口限制会自动调整满屏）
 * @param color 窗口背景色
 * @return unsigned char* 
 */
unsigned char* g_windowfill(const char* name,int width,int height,unsigned char color,int ins_x,int ins_y){
	if(height>sc->maxy) height = sc->maxy;
	if(width>sc->maxx) width = sc->maxx;
	unsigned char* p = (unsigned char*)mem_malloc(height*width);
	//阴影绘制
	for(int i = 0;i<SHADOW_PIX;i++)
		g_v_edgefill(p,width,i,i,1,width-2*i,height-2*i,i+16);
	//主窗体
	g_v_boxfill(p,width,SHADOW_PIX,SHADOW_PIX,width-2*SHADOW_PIX,height-2*SHADOW_PIX,color);
	//分割线
	g_v_boxfill(p,width,SHADOW_PIX,SHADOW_PIX+22,width-2*SHADOW_PIX,2,COLOR_LIGHT_GREY);
	//关闭
	g_v_boxfill(p,width,width-SHADOW_PIX-22,SHADOW_PIX+3,16,16,1);
	//窗口名
	g_shows(p,name,6,-21,(color+7)%14,width,ins_x,ins_y);
	return p;
}
/**
 * @brief 内容数组p上的相对位置加上一个字符,需要手动渲染
 * 
 * @param p 数组
 * @param c 字符
 * @param x0 窗口相对位置x（窗口总图层）
 * @param y0 窗口相对位置y（窗口总图层）
 * @param color 
 * @param sizex 原数组宽度
 */
void g_showc(unsigned char*p, char c, int x0, int y0, char color ,int sizex){
	
	extern char WORDS[128][16];
	for(int y = y0;y < y0+16;y++){
		for(int x = x0;x < x0+8;x++){
			if(WORDS[c][y-y0] & (0x80>>(x-x0))) p[y*sizex+x] = color;
		}
	}
	return;
}
/**
 * @brief 在某一图层的内容数组buf上的相对位置加上一个字符串，需要手动渲染
 * 
 * @param p 原数组
 * @param string 
 * @param x0 正文内容相对位置x
 * @param y0 正文内容相对位置y
 * @param color 
 * @param sizex 原图层宽度
 * @param ins_x 正文内容可用宽度
 * @param ins_y 正文内容可用高度
 * @return int 字符串长度，不包括\0
 */
int g_shows(unsigned char*p,const char* string, int x0, int y0, char color,int sizex,int ins_x,int ins_y){
	
	int count = 0;
	int x = x0;
	int y = y0;
	for(;*string;string++,x+=8,count++){
		if(x+8>ins_x){
			//超过正文内容显示区域，换行
			x = LS_INTERVAL;
			y += 16;
		}
		//todo 如果超过高度限制屏幕上滚
		g_showc(p,*string,x+SHADOW_PIX,y+SHADOW_PIX+WINDOWHEAD_PIX,color,sizex);
	}
	return count;
}
/**
 * @brief 初始化图层控制器
 * 
 */
void init_sheet_ctrl(){
	sc = (struct sheet_ctrl*)mem_malloc(sizeof(struct sheet_ctrl));
	sc->maxx = *((int*)0xff4);sc->maxy=*((int*)0xff8);
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
	//如果原层数和新层数相同不作操作
	if(old == z) return; 
	else if(old>z){
		//下调
		struct SHEET* temp = sc->sheet_zlevel[old];
		//将原层数到新层数之间的图层上移一层
		for(int i=old;i>z;i--){
			sc->sheet_zlevel[i] = sc->sheet_zlevel[i-1];
			sc->sheet_zlevel[i]->z ++;
		}
		//将原图层转移到目标层数
		sc->sheet_zlevel[z] = temp;
		sc->sheet_zlevel[z]->z = z;
		int x1 =  sc->sheets[index].x+sc->sheets[index].width;
		int y1 = sc->sheets[index].y+sc->sheets[index].height;
		for(int i = sc->sheets[index].y;i<y1&&i<sc->maxy;i++)
			for(int j=sc->sheets[index].x;j<x1&&j>=0&&j<sc->maxx;j++){
				//对图层范围内遍历，如果显示图层不是当前图层跳过
				if(screen_buf[i*sc->maxx+j]!=&(sc->sheets[index])) continue;
				else{
					//判断该像素是否有转移上去的图层覆盖，且不是透明色255。如果有就更新图层并渲染
					for(int k = old;k>=z;k--){
						int point = (i-sc->sheet_zlevel[k]->y)*sc->sheet_zlevel[k]->width+(j-sc->sheet_zlevel[k]->x);
						if(pix_in_sheet(j,i,k) && sc->sheet_zlevel[k]->buf[point]!=COLOR_TRANSP){
							screen_buf[i*sc->maxx+j] = sc->sheet_zlevel[k];
							screen[i*sc->maxx+j] = sc->sheet_zlevel[k]->buf[point];
							break;
						}
					}
				}
			}
		return;
	}
	else{
		//上调
		struct SHEET* temp = sc->sheet_zlevel[old];
		for(int i=old;i<z;i++){
			//将原层数到新层数之间的图层下调一层
			sc->sheet_zlevel[i] = sc->sheet_zlevel[i+1];
			sc->sheet_zlevel[i]->z--;
		}
		//将原图层转移到目标图层
		sc->sheet_zlevel[z] = temp;
		sc->sheet_zlevel[z]->z = z;
		int x1 =  sc->sheets[index].x+sc->sheets[index].width;
		int y1 = sc->sheets[index].y+sc->sheets[index].height;
		for(int i = sc->sheets[index].y;i<y1&&i<sc->maxy;i++)
			for(int j=sc->sheets[index].x;j<x1&&j>=0&&j<sc->maxx;j++){
				int point = (i-sc->sheets[index].y)*sc->sheets[index].width+(j-sc->sheets[index].x);
				//在图层范围内遍历，如果显示图层是当前图层跳过，已经显示的像素上调后肯定显示
				if(screen_buf[i*sc->maxx+j]==&(sc->sheets[index])) continue;
				//如果不是当前图层 且图层层数小于调整后层数 且当前图层该像素不是透明色 则更新图层并渲染
				else if(screen_buf[i*sc->maxx+j]->z<z && sc->sheet_zlevel[z]->buf[point]!=COLOR_TRANSP) {
					screen_buf[i*sc->maxx+j] = sc->sheet_zlevel[z];
					screen[i*sc->maxx+j] = sc->sheet_zlevel[z]->buf[point];
				}
			}
		return;
	}
}
void sheet_hide(int index){
	//如果本来就隐藏不作操作
	if(sc->sheets[index].z<0);
	else{
		//将该图层上方的所有图层整体下移一层
		for(int i = sc->sheets[index].z;i<sc->top;i++){
			sc->sheet_zlevel[i] = sc->sheet_zlevel[i+1];
			sc->sheet_zlevel[i]->z--;
		}
		// 总层数减1
		sc->sheet_zlevel[sc->top] = (struct SHEET*)NULL;
		sc->top--;
		int x1 =  sc->sheets[index].x+sc->sheets[index].width;
		int y1 = sc->sheets[index].y+sc->sheets[index].height;
		for(int i = sc->sheets[index].y;i<y1&&i<sc->maxy;i++)
			for(int j=sc->sheets[index].x;j<x1&&j>=0&&j<sc->maxx;j++){
				// 在隐藏的图层范围遍历，如果显示图层不是当前图层，不作操作
				if(screen_buf[i*sc->maxx+j]!=&(sc->sheets[index])) continue;
				else{
					// 如果是当前图层，在这一图层下方的图层中寻找该像素是否有其他图层 且不是255透明色
					for(int k = sc->sheets[index].z-1;k>=0;k--){
						int point = (i-sc->sheet_zlevel[k]->y)*sc->sheet_zlevel[k]->width+(j-sc->sheet_zlevel[k]->x);
						if(pix_in_sheet(j,i,k) && sc->sheet_zlevel[k]->buf[point]!=COLOR_TRANSP){
							screen_buf[i*sc->maxx+j] = sc->sheet_zlevel[k];
							screen[i*sc->maxx+j] = sc->sheet_zlevel[k]->buf[point];
							break;
						}
					}
					// 如果没有其它图层，该像素位置图层置空，渲染为默认黑色
					if(screen_buf[i*sc->maxx+j]==&(sc->sheets[index])){
						screen_buf[i*sc->maxx+j] = (struct SHEET*) NULL;
						screen[i*sc->maxx+j] = COLOR_BLACK;
					}
				}
			}
		sc->sheets[index].z = -1;
	}
	return;
}
void sheet_display(int index,int z){
	if(sc->sheets[index].z<0){
		//将上方的图层整体上移一层
		for(int i=sc->top+1;i>z;i--){
			sc->sheet_zlevel[i]=sc->sheet_zlevel[i-1];
			sc->sheet_zlevel[i]->z++;
		}
		//空出的第z层放入新图层
		sc->sheet_zlevel[z] = &(sc->sheets[index]);
		sc->sheets[index].z = z;
		//总图层加1
		sc->top ++;
	}
	int x1 = sc->sheets[index].x+sc->sheets[index].width;
	int y1 = sc->sheets[index].y+sc->sheets[index].height;
	//遍历新加图层范围重新渲染
	for(int i = max(0,sc->sheets[index].y);i<y1&&i<sc->maxy;i++)
		for(int j=max(sc->sheets[index].x,0);j<x1&&j<sc->maxx;j++){
			int point = (i-sc->sheets[index].y)*sc->sheets[index].width+(j-sc->sheets[index].x);
			//如果没有该像素处没有图层或者图层层数<=新加图层 & 不是透明色255
			if((sc->sheet_zlevel[z]->buf[point]!=COLOR_TRANSP ) && (!screen_buf[i*sc->maxx+j]||screen_buf[i*sc->maxx+j]->z<z)) {
				//记录为当前图层，渲染颜色
				screen_buf[i*sc->maxx+j] = sc->sheet_zlevel[z];
				screen[i*sc->maxx+j] = sc->sheet_zlevel[z]->buf[point];
			}
		}

	return;
}
/**
 * @brief 注册一个图层，返回其所在的sheets数组下标，-1失败
 * 
 * @param x 绝对坐标
 * @param y 绝对坐标
 * @param width 
 * @param height 
 * @param z 
 * @param buffer 
 * @param hwndp1 所绑定的窗口的hwnd+1
 * @return int 
 */
int sheet_malloc(int x,int y,int width,int height,int z,unsigned char* buffer,int hwndp1){
	for(int i=0;i<MAX_SHEETS;i++){
		if(sc->sheets[i].flag==0){
			sc->sheets[i].flag=hwndp1;
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
	screen_buf = (struct SHEET **)mem_malloc(sizeof(int[sc->maxy][sc->maxx]));
	return;
}
/**
 * @brief 更新图层（针对内容更新和移动）
 * 
 * @param index 图层编号
 * @param x0 相对图层左上角坐标
 * @param y0 相对图层左上角坐标
 * @param width 需要更新的宽度
 * @param height 需要更新的高度
 */
void sheet_refresh(int index,int x0,int y0,int width,int height){
	int y=y0+sc->sheets[index].y;
	int x=x0+sc->sheets[index].x;
	int x1 = x+width;
	int y1 = y+height;
	//遍历需要更新部分
	for(int i = y;i<y1&&i<sc->maxy;i++)
		for(int j=x;j<x1&&j>=0&&j<sc->maxx;j++){
			int point = i*sc->maxx+j;
			//如果该像素不是透明色255 且显示为当前图层或没有图层 ，记录为当前图层，渲染颜色
			int point2 = (i-sc->sheets[index].y)*sc->sheets[index].width+j-sc->sheets[index].x;
			if(sc->sheets[index].buf[point2] != COLOR_TRANSP){
				if(!screen_buf[point]) {
					screen_buf[point] = &(sc->sheets[index]);
					goto lable1;
				}
				
				if(screen_buf[point] == &(sc->sheets[index]))
lable1:				screen[point] = sc->sheets[index].buf[point2];
			}
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
	int old_x = sc->sheets[index].x;
	int old_y = sc->sheets[index].y;
	int x1 = old_x+sc->sheets[index].width;
	int y1 = old_y+sc->sheets[index].height;
	//sheet_hide(index);
	sc->sheets[index].x=x;
	sc->sheets[index].y=y;
	sheet_display(index,h);
	int param[4] = {old_y,y1,old_x,x1};
	
	for(int i = max(0,param[0]);i<param[1]&&i<sc->maxy;i++){
		for(int j=max(0,param[2]);j<param[3]&&j<sc->maxx;j++){
			for(int k = sc->sheets[index].z;k>=0;k--){
				if(screen_buf[i*sc->maxx+j]!=&(sc->sheets[index])) continue;
				else{
					int point = (i-sc->sheet_zlevel[k]->y)*sc->sheet_zlevel[k]->width+(j-sc->sheet_zlevel[k]->x);
					if(pix_in_sheet(j,i,k) && sc->sheet_zlevel[k]->buf[point]!=COLOR_TRANSP){
						screen_buf[i*sc->maxx+j] = sc->sheet_zlevel[k];
						screen[i*sc->maxx+j] = sc->sheet_zlevel[k]->buf[point];
						break;
					}
				}
			}
		}
	}

	return;
}