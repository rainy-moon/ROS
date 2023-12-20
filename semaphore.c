#include "ros.h"
struct semaphore* init_sem(int value,char* name){
	struct semaphore* sem = (struct semaphore*)mem_malloc(sizeof(struct semaphore));
	if(sem){
		sem->value=value;
		sem->name=name;
		init_simlist(&(sem->queue));
	}
	return sem;
}
void semWait(struct semaphore* sem){
	int flags = store_eflags();
	cli();
	struct node* temp = nodecpy((struct node*) multipc_ctrl.pc);
	while(sem->value <= 0){
		simlist_sortedinsert(&(sem->queue),temp,4);
		sem->value--;
		PSleep((struct prograss*)temp);
	}
	mem_free((void *)temp,sizeof(temp));
	sem->value--;
	load_eflags(flags);
	return;
}

void semSignal(struct semaphore* sem){
	int flags = store_eflags();
	cli();
	if(sem->queue.size>0){
		struct node* p = simlist_delete(&(sem->queue),0);
		sem->value++;
		PAwake((struct prograss*)p);
	}
	else{
		sem->value++;
	}
	load_eflags(flags);
	return;
}

void clear_sem(struct semaphore* sem){
	simlist_free(&(sem->queue));
	mem_free((void*)sem,sizeof(struct semaphore));
	return;
}