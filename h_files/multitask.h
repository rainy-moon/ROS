#ifndef _PROGRASSH_
#define _PROGRASSH_
#include "ros.h"
#define MAX_PROCESS				32
#define SWITCH_TASK_INTERVEL 	2
#define SWITCH_TASK_DARA 		1
#define RUNNING			1
#define PC				1	
#define RUNABLE			2
#define PR				2
#define NEWC			3
#define PN				3
//struct define
struct TSS{
	int backlink;
	int esp0,ss0,esp1,ss1,esp2,ss2;
	int gr3;
	int eip,eflags;
	int eax,ecx,edx,ebx,esp,ebp,esi,edi;
	int es,cs,ss,ds,fs,gs;
	int ldtr;
	int iomap;
};
struct prograss{
	int statu;
	int flags;
	int parent;
	int pid;
	int level;
	struct TSS tss;
	int reserve[225];
}prograsses[MAX_PROCESS];
struct prograss_ctrl{
	int stt_tid;			//switch task timer tid
	int total_prograsses;
	struct prograss *pc; 	//当前进行进程
	struct SIMLIST pr;		//就绪进程链表
	struct SIMLIST ps;		//中断挂起链表
	struct SIMLIST pd;		//不可中断挂起链表
	struct SIMLIST pz;		//僵尸进程链表
	struct SIMLIST pn;		//创建或等待就绪链表
}multipc_ctrl;
//global define

//function define
void init_TSS(struct TSS* tss);
int create_task(int funcaddr,int level,int flags);
void regtask(struct GDT_SEG* gs,int pid,unsigned int limit,unsigned int settings);
int init_multipc_ctrl();
#endif