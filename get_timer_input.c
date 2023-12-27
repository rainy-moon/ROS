#include "ros.h"
/**
 * @brief Get the timer input object
 * @note 34 10s计时
 * @note 1 任务切换
 * @note 2 光标闪烁
 */
void get_timer_input(){
	if(io_buffer_num(&tm_buffer_ctrl)){
		int data = io_buffer_pop(&tm_buffer_ctrl);
		switch(data){
			case 34:
			{
				char test[20];
				my_strcpy(test,"mouse",20); 
				my_sprintf(s,"5s %d %d %s",time_count,windows[2].cursor_y,test);
				win_showsln(2,s,COLOR_BLACK);
				time_count = 0;
				break;
			}
			case 2:
				cursor_update();
				break;
		}
	}
	else{
		ISleep(&(prograsses[2]));
	}
	return;
}