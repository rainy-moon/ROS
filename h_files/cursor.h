#ifndef _CURSOR_
#define _CURSOR_
#include"ros.h"
// struct define
// global define 
//光标状态，0（闪烁）隐藏，1（闪烁）显示，2（永久）隐藏，3（永久）显示
int cursor_state = 0;
//确保光标状态临界区
struct semaphore* sem_cursor;

// function define
/**
 * @brief 光标更新，绑定时钟信号2
 * 
 */
void cursor_update();
/**
 * @brief 设置光标为永久隐藏，此时时钟信号2仍然执行
 * 
 */
void cursor_hide();
/**
 * @brief 设置光标为永久显示，此时时钟信号2仍然执行
 * 
 */
void cursor_shown();
/**
 * @brief 设置光标暂停显示。先关闭显示，再删去时钟信号2，用于有io输入显示时
 * 
 */
void cursor_pause();
/**
 * @brief 设置光标恢复。重新注册时钟信号2，光标保持暂停前的模式，用于io输入显示结束
 * 
 */
void cursor_resume();
#endif