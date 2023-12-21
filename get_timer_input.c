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
				if(cursor_state==0||cursor_state==2){
					cursor_state = (cursor_state/2)+1;
					g_showc(sc->sheets[focused_window->sheet_index].buf,2,focused_window->cursor_x,focused_window->cursor_y,COLOR_BLACK,focused_window->width);
					sheet_refresh(focused_window->sheet_index,focused_window->cursor_x,focused_window->cursor_y,8,16);
				}
				else{
					cursor_state = (cursor_state*4)/3-1;
					//todo 这里以后要改成窗口背景色
					g_showc(sc->sheets[focused_window->sheet_index].buf,2,focused_window->cursor_x,focused_window->cursor_y,COLOR_WHITE,focused_window->width);
					sheet_refresh(focused_window->sheet_index,focused_window->cursor_x,focused_window->cursor_y,8,16);
				}
				break;
		}
	}
	else{
		ISleep(&(prograsses[2]));
	}
	return;
}