#ifndef _TIMERH_
#define _TIMERH_
#include "ros.h"
#define MAX_TIMER		256
// struct define
struct timer_ctrl{
	struct SIMLIST timelist;
	int time;
	int num;
}tc;
/**
 * @brief 计时器
 * @param timeout 超时时的当前计数（time+tc.time）
 * @param time 计时长度
 * @param data 超时时向缓冲区内写的内容
 * @param flags 状态标志位
 * 0bit 0循环任务 1单次任务
 */
struct timer{
	unsigned int timeout;
	unsigned int time;
	int flags;
	int tid;
	int data;
	int reserve[250];
	struct timer* next;
};

// global define
unsigned int time_count = 0;

// function define
int init_timerctrl();
int timer_malloc(unsigned int timeout,int flags,int data);
int timer_delete(int tid);
int timer_reset(int tid,unsigned int timeout,int flags,int data);
int timer_toc();
#endif