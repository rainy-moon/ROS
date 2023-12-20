#ifndef _SEMAPHOREH_
#define _SEMAPHOREH_
#include"ros.h"
// struct define
struct semaphore{
	int value;
	struct SIMLIST queue;
	char* name; //用来debug
};
//global define
struct semaphore* sem1;
//function define
/**
 * @brief 初始化信号量
 * @param value 信号量的值（大于等于0）
 * @param name 信号量名，用于debug
 * @return 成功返回信号量地址，失败返回0
 */
struct semaphore* init_sem(int value,char* name);
/**
 * @brief 采用mesa语义注意唤醒后不会立刻执行，只进入就绪队列。
 * 执行时仍然需要判断是否满足执行条件。
 * @param sem 信号量指针
 */
void semWait(struct semaphore* sem);
void semSignal(struct semaphore* sem);
void clear_sem(struct semaphore* sem);
#endif