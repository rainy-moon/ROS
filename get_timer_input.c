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
				//semSignal(sem1);
				my_sprintf(s,"10sec %d",time_count);
				win_showsln(2,s,COLOR_BLACK);
				time_count = 0;
				//semWait(sem1);
				break;
			case 2:
			//todo 光标有点问题，可能需要重新实现窗口画布才能解决
				
				break;
		}
	}
	else{
		ISleep(&(prograsses[2]));
	}
	return;
}