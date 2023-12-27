#ifndef _CONSOLE_
#define _CONSOLE_
#include"ros.h"
//struct define
//global define
char command[64];
struct console_command{
	int point;
	int statu; //0 未执行 1 正在执行 2 问答模式
	int hwnd;
}cc;
//function define
void init_console_command();
/**
 * @brief 将键盘输入放入缓冲区command
 * 
 * @param c 
 */
void console_inputchar(char c);
/**
 * @brief 分析command指令，并根据相应指令执行
 * 
 */
void ConsoleExec();
#endif