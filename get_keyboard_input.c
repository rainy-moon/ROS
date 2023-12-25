#include "ros.h"
void get_keyboard_input(){
	if(io_buffer_num(&kb_buffer_ctrl)) {
		int data = io_buffer_pop(&kb_buffer_ctrl);
		if(data!=-1&&data<0x44&&dictionary[data]){
			// my_sprintf(s,"kb: %c",dictionary[data]);
			// win_showsln(1,s,7);
			win_showc(focused_window->hwnd,dictionary[data],(focused_window->bg_color+7)%14);
		}
	}
	else ISleep(&prograsses[4]);
	return;
}