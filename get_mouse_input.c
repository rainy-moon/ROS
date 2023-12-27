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
			//else{ISleep(&prograsses[3]); break;}
			else{ISleep(&(prograsses[mouse_input_pid-1])); break;}
		case 1:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1 && (data & 0xc8)==0x8){
					mouse.mouse_state[0]=(unsigned char)data;
					ms_state = 2;
				}
				else break;
			}
			else{ISleep(&(prograsses[mouse_input_pid-1])); break;}
		case 2:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1){
					mouse.mouse_state[1]=(unsigned char)data;
					ms_state = 3;
				}
				else break;
			}
			//else {ISleep(&prograsses[mouse_input_pid-1]); break;}
		case 3:
			if(io_buffer_num(&ms_buffer_ctrl)){
				int data = io_buffer_pop(&ms_buffer_ctrl);
				if(data!=-1){
					mouse.mouse_state[2]=(unsigned char)data;
					ms_state = 1;
					mouse_decode();					
					IAwake(&prograsses[mouse_detector_pid-1]);
				}
				else break;
			}
			//else {ISleep(&prograsses[mouse_input_pid-1]); break;}
	}
	return ms_state;	
}

void mousedetector(){
	int i = sc->top-1;
	for(;i>=0;i--)
		if(pix_in_sheet(mouse.posx,mouse.posy,i)) break;
	if(i==-1){
		if(mouse.hwndp1){
			windows[mouse.hwndp1-1].statu&=0xffffff01;
			windows[mouse.hwndp1-1].mouse_x=0x7fffffff;
			windows[mouse.hwndp1-1].mouse_y=0x7fffffff;
			mouse.hwndp1=0;
		}
	}
	else{
		if(mouse.hwndp1 != sc->sheet_zlevel[i]->flag){
			if(!mouse.hwndp1){
				mouse.hwndp1 = sc->sheet_zlevel[i]->flag;
				windows[mouse.hwndp1-1].statu|=0x00000002;
				windows[mouse.hwndp1-1].mouse_x = mouse.posx-windows[mouse.hwndp1-1].x0;
				windows[mouse.hwndp1-1].mouse_y = mouse.posy-windows[mouse.hwndp1-1].y0;
			}
			else if((windows[mouse.hwndp1-1].statu&0xff)>>1==1){
				windows[mouse.hwndp1-1].mouse_x=0x7fffffff;
				windows[mouse.hwndp1-1].mouse_y=0x7fffffff;
				windows[mouse.hwndp1-1].statu&=0xffffff01;
				mouse.hwndp1 = sc->sheet_zlevel[i]->flag;
				windows[mouse.hwndp1-1].mouse_x = mouse.posx-windows[mouse.hwndp1-1].x0;
				windows[mouse.hwndp1-1].mouse_y = mouse.posy-windows[mouse.hwndp1-1].y0;
				windows[mouse.hwndp1-1].statu|=0x00000002;
			}
		}
		
		int statu = (windows[mouse.hwndp1-1].statu&0xff)>>1;
		struct window* w = &(windows[mouse.hwndp1-1]);
		//检查鼠标状态
		switch(mouse.btn){
			case 0:
			{
				if(statu==2){
					//点击
					w->statu&=0xffffff01;
					w->statu|=0x00000002;
					if(((w->statu>>8)&0xff) != 0){
						cursor_pause();
						if(w->statu&0x0000ff00) focused_window = w;
						sheet_updatez(w->sheet_index,sc->top-1);
						cursor_resume();
					}
					//todo 点击区域事件检查
					if((w->statu>>8)&0xff !=2 
					&& between(w->mouse_x,w->width-SHADOW_PIX-22,w->width-SHADOW_PIX-6) 
					&& between(w->mouse_y,SHADOW_PIX+3,SHADOW_PIX+19)){
						int f=0;
						if(focused_window==w) {cursor_pause();f=1;}
						win_free(w->hwnd);
						focused_window = &windows[get_hwnd_by_name("Console")];
						if(f) cursor_resume();
						mouse.hwndp1 = 0;
					}
				}else if(statu==4){
					//松开拖动
					w->statu&=0xffffff01;
					w->statu|=0x00000002;
				}
				break;
			}
			case 1:
			{
				if(statu == 1 ){
					w->statu&=0xffffff01;
					w->statu|=0x00000004;
					
				}
				else if((statu==2||statu==4)&&(mouse.x||mouse.y)){
					//拖动
					w->statu&=0xffffff01;
					w->statu|=0x00000008;
					if(((w->statu>>8)&0xff) != 0){
						cursor_pause();
						// int eflag = store_eflags();
						// cli();
						w->x0+=mouse.posx-w->x0-w->mouse_x;
						w->y0+=mouse.posy-w->y0-w->mouse_y;
						sheet_slide(w->x0,w->y0,w->sheet_index);
						// load_eflags(eflag);
						cursor_resume();
					}
				}
				break;
			}
		}
		w->mouse_x=mouse.posx-w->x0;
		w->mouse_y=mouse.posy-w->y0;	
	}
	my_sprintf(s,"mouse %d %d %d",mouse.btn,mouse.hwndp1-1,(windows[mouse.hwndp1-1].statu&0xff)>>1);
	win_showslr(1,s,COLOR_BLACK);
	ISleep(&prograsses[mouse_detector_pid-1]);
}