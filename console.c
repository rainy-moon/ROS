#include"ros.h"
void init_console_command(){
	cc.point = 0;
	cc.statu = 0;
	cc.hwnd = get_hwnd_by_name("Console");
	int pid_5 = create_task("console",(int)&ConsoleExec,0,0);
	if(pid_5) regtask(pid_5,103,AR_TSS32);
}
void console_inputchar(char c){
	switch(c){
		case '\n':
		{
			command[cc.point]='\0';
			cc.statu=1;
			IAwake(&prograsses[5]);
			cc.point=0;
			break;
		}
		case '\b':
		{
			cc.point--;		
		}
		default:
		{
			command[cc.point++]=c;
		}
	}
}

void ConsoleExec(){
	while(1){
		if(cc.statu == 0);
		else if(cc.statu){
			splitby(command,' ');
			if(!my_strcmp(ss[1],"clr",3)){
				//不关注参数，console清屏
				if(focused_window==(windows+cc.hwnd)) cursor_pause;
				g_v_boxfill(sc->sheets[windows[cc.hwnd].sheet_index].buf,windows[cc.hwnd].width,SHADOW_PIX,WINDOWHEAD_PIX+SHADOW_PIX,windows[cc.hwnd].ins_width,windows[cc.hwnd].ins_height,COLOR_BLACK);
				sheet_refresh(windows[cc.hwnd].sheet_index,SHADOW_PIX,WINDOWHEAD_PIX+SHADOW_PIX,windows[cc.hwnd].ins_width,windows[cc.hwnd].ins_height);
				windows[cc.hwnd].cursor_x = LS_INTERVAL;
				windows[cc.hwnd].cursor_y = RS_INTERVAL;
				if(focused_window==(windows+cc.hwnd)) cursor_resume;
				cc.point = 0;
			}else if(!my_strcmp(ss[1],"open",cc.hwnd)){
				//打开一个新默认普通窗口（类型为1，（200,100）500*400），后跟一个参数作为窗口名
				if((int)ss[0]<2 ||my_strlen(ss[2])>20 
				||!my_strcmp(ss[2],"debug",5) 
				||!my_strcmp(ss[2],"console",7) 
				||!my_strcmp(ss[2],"desktop",7)
				){
					win_showsln(cc.hwnd,">Forbidden name",COLOR_WHITE);
				}
				else{
					win_create(ss[2],200,100,500,400,sc->top,COLOR_WHITE,1);
				}
			}else if(!my_strcmp(ss[1],"mem",3)){
				//内存检查
				int eflag = store_eflags();
				cli();
				my_sprintf(s,">total memory: %d KB",memcheck(0x520000,0x20000000));
				load_eflags(eflag);
				win_showsln(cc.hwnd,s,COLOR_WHITE);
			}else if(!my_strcmp(ss[1],"hi",2)){
				//没啥用的好玩命令，检查第二参数
				if((int)ss[0]<2 ||!my_strcmp(ss[2],"zero",4)){
					win_showsln(cc.hwnd,">Who are you calling?",COLOR_WHITE);
				}else{
					win_showsln(cc.hwnd,">Hi, what can I do?",COLOR_WHITE);
					cc.statu = 2;
				}
			}else{
				if(cc.statu == 1) win_showsln(cc.hwnd,">Unrecognized command",COLOR_WHITE);
				else if(cc.statu == 2) {
					win_showsln(cc.hwnd,">Why do you think I can understand that? DO IT YOURSELF!",COLOR_WHITE);
					cc.statu = 1;
				}
			}
		}
		win_showc(cc.hwnd,'>',COLOR_WHITE);
		ISleep(&prograsses[5]);
	}
}