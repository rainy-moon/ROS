#include"ros.h"


void cursor_update(){
	semWait(sem_cursor);
	if(cursor_state==0||cursor_state==2){
		cursor_state = (cursor_state/2)+1;
		g_showc(sc->sheets[focused_window->sheet_index].buf,2,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,(focused_window->bg_color+7)%14,focused_window->width);
		sheet_refresh(focused_window->sheet_index,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);
	}
	else{
		cursor_state = (cursor_state*4)/3-1;
		//todo 这里以后要改成窗口背景色
		g_showc(sc->sheets[focused_window->sheet_index].buf,2,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,focused_window->bg_color,focused_window->width);
		sheet_refresh(focused_window->sheet_index,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);
	}
	semSignal(sem_cursor);
}

void cursor_hide(){ 
	semWait(sem_cursor);
	if(cursor_state == 0 || cursor_state == 2);
	else{
		//todo 这里以后要改成窗口背景色
		g_showc(sc->sheets[focused_window->sheet_index].buf,2,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,focused_window->bg_color,focused_window->width);
		sheet_refresh(focused_window->sheet_index,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);
	}
	cursor_state = 2;
	semSignal(sem_cursor);
}

void cursor_shown(){
	semWait(sem_cursor);
	if(cursor_state == 1||cursor_state == 3);
	else{
		g_showc(sc->sheets[focused_window->sheet_index].buf,2,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,(focused_window->bg_color+7)%14,focused_window->width);
		sheet_refresh(focused_window->sheet_index,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);
	}
	cursor_state = 3;
	semSignal(sem_cursor);
}

void cursor_pause(){
	semWait(sem_cursor);
	g_showc(sc->sheets[focused_window->sheet_index].buf,2,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,focused_window->bg_color,focused_window->width);
	sheet_refresh(focused_window->sheet_index,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);

	if(cursor_tid){
		timer_delete(cursor_tid);
		cursor_tid = 0;
	}
	semSignal(sem_cursor);
}

void cursor_resume(){
	semWait(sem_cursor);
	// if(cursor_state == 0 || cursor_state == 2){
	// 	g_showc(sc->sheets[focused_window->sheet_index].buf,2,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,COLOR_WHITE,focused_window->width);
	// 	sheet_refresh(focused_window->sheet_index,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);
	// }
	// else{
	// 	//todo 这里以后要改成窗口背景色
	// 	g_showc(sc->sheets[focused_window->sheet_index].buf,2,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,COLOR_BLACK,focused_window->width);
	// 	sheet_refresh(focused_window->sheet_index,focused_window->cursor_x+SHADOW_PIX,focused_window->cursor_y+SHADOW_PIX+WINDOWHEAD_PIX,8,16);
	// }
	cursor_tid = timer_malloc(50,0,2);
	semSignal(sem_cursor);
}