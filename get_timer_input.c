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
				cursor_pause();
				my_sprintf(s,"10s %d %d",time_count,cursor_state);
				win_showsln(2,s,COLOR_BLACK);
				cursor_resume();
				time_count = 0;
				break;
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