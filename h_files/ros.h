#ifndef _ROSH_
#define _ROSH_
#define NULL  			0
//多任务状态定义

//sysfunc.asm 汇编定义函数，寄存器级操作

#include "sysasm.h"

//graphAfont.c 图像画面操作函数

#include "graphAfont.h"

//窗口 windows.c

#include "windows.h"

//c库复现
#include "lib.h"

//system.c系统设置函数
#include "system.h"

//iobuffer.c io缓冲区函数
#include "iobuffer.h"

//键盘鼠标计时器读取控制任务
#include "get_keyboard_input.h"
#include "get_mouse_input.h"
#include "get_timer_input.h"

//memory.c 系统内存分配函数
#include "memory.h"

//simlist.c 模拟链表函数

#include "simlist.h"
//timerAmultitask.c 计时器和多任务

#include "timer.h"
#include "multitask.h"
//semaphore.c 信号量实现
#include "semaphore.h"

//cursor.c 光标显示相关函数
#include "cursor.h"

//系统任务
void SwitchTaskTest();
void TimerInputAction();
void MouseInputAction();
void KeyInputAction();
//全局量
unsigned char s[20];


#endif