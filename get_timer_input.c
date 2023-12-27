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
				int debug = -1;
				debug = get_hwnd_by_name("performance");
				if(debug!=-1){
					my_sprintf(s,"5s %d times calculation",time_count);
					win_showsln(debug,s,COLOR_BLACK);
				}
				debug = get_hwnd_by_name("windows");
				if(debug!=-1){
					show_windows(debug);
				}
				debug = get_hwnd_by_name("processes");
				if(debug!=-1){
					show_prograsses(debug);
				}
				debug = get_hwnd_by_name("timers");
				if(debug!=-1){
					show_timers(debug);
				}
				time_count = 0;
				break;
			}
			case 2:
				cursor_update();
				break;
		}
	}
	else{
		ISleep(&(prograsses[timer_input_pid-1]));
	}
	return;
}