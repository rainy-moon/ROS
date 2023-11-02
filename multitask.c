#include "ros.h"
int init_multipc_ctrl(){
	prograsses[0].pid = 1;
	prograsses[0].tss.iomap=0x40000000;
	prograsses[0].tss.ldtr=0;
	prograsses[0].statu = RUNNING;
	init_simlist(&(multipc_ctrl.pr));
	init_simlist(&(multipc_ctrl.ps));
	init_simlist(&(multipc_ctrl.pd));
	init_simlist(&(multipc_ctrl.pz));
	init_simlist(&(multipc_ctrl.pn));
	multipc_ctrl.total_prograsses = 0;
	multipc_ctrl.pc = prograsses;
	multipc_ctrl.stt_tid = timer_malloc(SWITCH_TASK_INTERVEL,0,SWITCH_TASK_DARA);
	regtask(gs+3,1,103,AR_TSS32);
	load_tr(3<<3);
	return 1;
}

void init_TSS(struct TSS* tss){
	for(int i=0;i<26;i++,(int*)tss++)
		*(int*)tss=0;
	return;
}

int create_task(int funcaddr,int level,int flags){
	int i=0;
	for(;i<MAX_PROCESS;i++)
		if(prograsses[i].pid==0) break;
	struct TSS* base = &(prograsses[i].tss);
	init_TSS(base);
	void * stack = mem_malloc(16*1024);
	if(stack) base->esp = (int)stack+16*1024;
	else return 0;
	base->es = 1<<3;
	base->cs = 2<<3;
	base->ss = 1<<3;
	base->ds = 1<<3;
	base->fs = 1<<3;
	base->gs = 1<<3;
	base->eip = funcaddr-0x101000;
	base->eflags=0x00000202;
	prograsses[i].parent = multipc_ctrl.pc->pid;
	prograsses[i].level = level;
	prograsses[i].flags = flags;
	prograsses[i].statu = PN;
	prograsses[i].pid=i+1;
	return prograsses[i].pid;
}

void regtask(struct GDT_SEG* gs,int pid,unsigned int limit,unsigned int settings){
	set_gdt_segment(gs,limit,(unsigned int)&(prograsses[pid-1].tss),settings,0);
	prograsses[pid-1].statu = RUNABLE;
	multipc_ctrl.total_prograsses++;
	return;
}

void change_task(){
	
	return;
}

void PSleep(struct prograss* p){
	int index = simlist_find(&(multipc_ctrl.pr),p->pid,3);
	if(index==-1) return;
	else {
		simlist_delete(&(multipc_ctrl.pr),index);
		simlist_sortedinsert(&(multipc_ctrl.ps),(struct node*)(prograsses+p->pid-1),4);
		if(p==multipc_ctrl.pc){
			
		}
	}
	return;
}