#include "ros.h"
void get_keyboard_input(){
	if(io_buffer_num(&kb_buffer_ctrl)) {
		int data = io_buffer_pop(&kb_buffer_ctrl);
		if(data!=-1){
			my_sprintf(s,"kb_num : %d",data);
			win_showsln(1,s,7);
		}
	}
	return;
}