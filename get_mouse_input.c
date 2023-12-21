#include"ros.h"
void mouse_decode(){
	mouse.btn = mouse.mouse_state[0] & 0x07;
	mouse.x = mouse.mouse_state[1];
	mouse.y = mouse.mouse_state[2];
	if(mouse.x && mouse.mouse_state[0] & 0x10)
		mouse.x |= 0xffffff00;
	if(mouse.y && mouse.mouse_state[0] & 0x20)
		mouse.y |= 0xffffff00;
	mouse.y = -mouse.y;
	//打印字符和更新鼠标，可能以后要转移到图层控制模块
	
	mouse.posx=(mouse.posx+mouse.x);
	mouse.posy=(mouse.posy+mouse.y);
	if(mouse.posx<0) mouse.posx=0;
	if(mouse.posx>=sc->maxx) mouse.posx=sc->maxx-1;
	if(mouse.posy<0) mouse.posy=0;
	if(mouse.posy>=sc->maxy) mouse.posy=sc->maxy-1;
	sheet_slide(mouse.posx,mouse.posy,windows[mouse.hwnd].sheet_index);
}

int get_mouse_input(int ms_state){
	switch (ms_state){
		case 0:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1){
					ms_state = 1;
				}
				else break;
			}
			else{ISleep(&prograsses[3]); break;}
		case 1:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1 && (data & 0xc8)==0x8){
					mouse.mouse_state[0]=(unsigned char)data;
					ms_state = 2;
				}
				else break;
			}
			else{ISleep(&prograsses[3]); break;}
		case 2:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1){
					mouse.mouse_state[1]=(unsigned char)data;
					ms_state = 3;
				}
				else break;
			}
			else {ISleep(&prograsses[3]); break;}
		case 3:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1){
					mouse.mouse_state[2]=(unsigned char)data;
					ms_state = 1;
					mouse_decode();
				}
				else break;
			}
			else {ISleep(&prograsses[3]); break;}
	}
	return ms_state;	
}