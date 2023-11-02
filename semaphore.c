#include "ros.h"
void semWait(struct semaphore* sem){
	int flags = store_eflags();
	cli();
	if(sem->value == 0){
		simlist_sortedinsert(&(sem->queue),(struct node*)multipc_ctrl.pc,4);
		
	}
	return;
}