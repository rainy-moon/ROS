#include "ros.h"
void get_keyboard_input(){
	if(io_buffer_num(&kb_buffer_ctrl)) {
		int data = io_buffer_pop(&kb_buffer_ctrl);
		if(data!=-1&&data<0x44&&dictionary[data]){
			if((focused_window->statu>>8&0xff)==2)
				console_inputchar(dictionary[data]);
			win_showc(focused_window->hwnd,dictionary[data],(focused_window->bg_color+7)%14);
		}
	}
	else ISleep(&prograsses[keyboard_input_pid-1]);
	return;
}