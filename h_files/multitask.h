#ifndef _PROGRASSH_
#define _PROGRASSH_
#include "ros.h"
#define MAX_PROCESS				32
#define SWITCH_TASK_INTERVEL 	1
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
	char name[20];
	int reserve[220];
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
/**
 * @brief 将tss初始化成全0
 * 
 * @param tss 
 */
void init_TSS(struct TSS* tss);
/**
 * @brief 创建任务
 * @param name 任务名
 * @param funcaddr 函数地址
 * @param level 优先级
 * @param flags 标志
 * @return int 失败返回0，成功返回进程pid
 * @note 目前默认代码段cs:2<<3 地址0x101000；
 * @note 目前默认数据段及其他初代码段1<<3 地址0x0
 * @note 自动分配16k栈空间
 */
int create_task(char* name,int funcaddr,int level,int flags);
/**
 * @brief 注册任务,搜索是否能分配任务结构体，填写相应结构体描述，返回pid
 * 
 * @param gs 段号
 * @param pid 任务id
 * @param limit 限长
 * @param settings 设置
 * @return 
 */
void regtask(int pid,unsigned int limit,unsigned int settings);
int init_multipc_ctrl();
/**
 * @brief 进程切换,将pc改成待切换进程，执行任务切换函数
 * @param pid 进程pid
 */
void change_task(int pid);
/**
 * @brief 不可中断休眠
 *  
 * @param p 进程控制块地址
 * @note 此函数在中断关闭下运行
 */
void PSleep(struct prograss* p);
/**
 * @brief 不可中断唤醒
 * 
 * @param p 进程控制块地址
 */
void PAwake(struct prograss* p);
/**
 * @brief 可中断休眠
 * @note 此函数在中断关闭下运行
 * @param p 
 */
void ISleep(struct prograss* p);
/**
 * @brief 可中断唤醒
 * 
 * @param p 
 */
void IAwake(struct prograss* p);

#endif