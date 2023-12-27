#include"ros.h"
void show_prograsses(int hwnd){
	win_showsln(hwnd,"PROGRASS:",COLOR_BLACK);
	win_showsln(hwnd,"------------------------------",COLOR_BLACK);
	win_showsln(hwnd,"name | pid | statu | flags | parent",COLOR_BLACK);
	win_showsln(hwnd,"------------------------------",COLOR_BLACK);
	for(int i=0;i<MAX_PROCESS;i++){
		if(prograsses[i].pid){
			my_sprintf(s,"%s | %d | %d | %d | %d",
			prograsses[i].name,
			prograsses[i].pid,
			prograsses[i].statu,
			prograsses[i].flags,
			prograsses[i].parent
			);
			win_showsln(hwnd,s,COLOR_BLACK);
		}
	}
	win_showsln(hwnd,"------------------------------",COLOR_BLACK);
}
void show_windows(int hwnd){
	win_showsln(hwnd,"WINDOW:",COLOR_BLACK);
	win_showsln(hwnd,"------------------------------",COLOR_BLACK);
	win_showsln(hwnd,"name | hwnd | statu | sheet_index | bg_color",COLOR_BLACK);
	win_showsln(hwnd,"------------------------------",COLOR_BLACK);
	for(int i=0;i<MAX_WINDOWS;i++){
		if(windows[i].statu&0x1){
			my_sprintf(s,"%s | %d | %d | %d | %d",
			windows[i].name,
			windows[i].hwnd,
			windows[i].statu,
			windows[i].sheet_index,
			windows[i].bg_color
			);
			win_showsln(hwnd,s,COLOR_BLACK);
		}
	}
	win_showsln(hwnd,"------------------------------",COLOR_BLACK);
}
void show_timers(int hwnd){
	win_showsln(hwnd,"TIMER:",COLOR_BLACK);
	win_showsln(hwnd,"------------------------------",COLOR_BLACK);
	win_showsln(hwnd,"tid | time | timeout | flags | data",COLOR_BLACK);
	win_showsln(hwnd,"------------------------------",COLOR_BLACK);
	struct node* p=tc.timelist.head;
	while(p){
		my_sprintf(s,"%d | %d | %d | %d | %d",
		((struct timer*)p)->tid,
		((struct timer*)p)->time,
		((struct timer*)p)->timeout,
		((struct timer*)p)->flags,
		((struct timer*)p)->data
		);
		win_showsln(hwnd,s,COLOR_BLACK);
		p=p->next;
	}
	win_showsln(hwnd,"------------------------------",COLOR_BLACK);
}