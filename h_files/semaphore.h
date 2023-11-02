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

//function define
//P()
void semWait(struct semaphore* sem);
void semSignal(struct semaphore* sem);
#endif