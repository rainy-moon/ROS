#include "ros.h"
/**
 * @brief Get the timer input object
 * @note 34 10s计时
 * @note 1 任务切换。目前只实现两个任务的简单切换
 */
void get_timer_input(){
	if(io_buffer_num(&tm_buffer_ctrl)){
		int data = io_buffer_pop(&tm_buffer_ctrl);
		switch(data){
			case 34:
				my_sprintf(s,"10sec %d",time_count);
				win_showsln(2,s,COLOR_BLACK);
				time_count = 0;
				break;
		}
	}
	return;
}