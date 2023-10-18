#include"ros.h"
/**
 * @brief 初始化全局计时器控制器
 * 成功返回0，失败返回1
 * @return int 
 */
int init_timerctrl(){
	init_simlist(&(tc.timelist));
	struct timer* guard = mem_malloc(sizeof(struct timer));
	if(!guard) return 0;
	guard->time = 0xffff;
	guard->timeout = 0xffff;
	guard->flags = 0;
	guard->tid = 0x0;
	guard->data = 0xff;
	cli();
	simlist_sortedinsert(&(tc.timelist),(struct node*)guard,0);
	tc.num=1;
	tc.time=0;
	sti();
	return 1;
}
/**
 * @brief 新加计时器
 * 失败返回0，成功返回计时器tid
 * @param time 计时10ms*time
 * @param flags 
 * @param data 
 * @return int 
 */
int timer_malloc(unsigned int time,int flags,int data){
	if(tc.num>=256) return 0;
	struct timer* t =mem_malloc(sizeof(struct timer));
	if(!t) return 0;
	t->flags = flags;
	t->data = data;
	t->time = time;
	t->tid = tc.num;
	cli();
	t->timeout=time+tc.time;
	simlist_sortedinsert(&(tc.timelist),(struct node*)t,0);
	tc.num++;
	sti();
	return t->tid;
}
/**
 * @brief 删除计时器
 * 失败返回0，成功过返回1
 * @param tid 计时器的tid
 * @return int 
 */
int timer_delete(int tid){
	int index=0;
	struct timer* t=(struct timer*)(tc.timelist.head);
	for(;index<tc.num;index++){
		if(t->tid==tid) break;
		else t = t->next;
	}
	if(index >= tc.num) return 0;
	if(!simlist_delete(&(tc.timelist),index)) return 0;
	tc.num--;
	return 1;
}
/**
 * @brief 重新设置计时器
 * 失败返回0，成功返回1
 * @param tid 
 * @param timeout 计时10ms*timeout
 * @param flags 
 * @param data 
 * @return int 
 */
int timer_reset(int tid,unsigned int time,int flags,int data){
	int index=0;
	struct timer* t=(struct timer*)(tc.timelist.head);
	for(;index<tc.num;index++){
		if(t->tid==tid) break;
		else t = t->next;
	}
	if(index >= tc.num) return 0;
	t->flags = flags;
	t->data = data;
	t->time = time;
	cli();
	t->timeout = time+tc.time;
	sti();
	return 1;
}
/**
 * @brief time out current当头任务超时，入缓冲，
 * 判断设置如果是重复任务重新加入计时器
 * 删除计时器
 * @note 此函数在中断中执行
 * @return int 
 */
int timer_toc(){
	int temp = ((struct timer*)(tc.timelist.head))->data;
	if((((struct timer*) tc.timelist.head)->flags&0x00000001) == 0){
		int time = ((struct timer*)tc.timelist.head)->time;
		int flags = ((struct timer*)tc.timelist.head)->flags;
		int data = ((struct timer*)tc.timelist.head)->data;
		if(!(timer_delete(((struct timer*)tc.timelist.head)->tid) && timer_malloc(time,flags,data)))
			return 0;
	}
	else if(!timer_delete(((struct timer*)tc.timelist.head)->tid)) return 0;
	if(temp == SWITCH_TASK_DARA && multipc_ctrl.total_prograsses>1){
		//权宜之计，只有两个任务，后续要实现就绪队列选择
		int t = (multipc_ctrl.pc->pid%2);
		return t+3;
	}else{
		if(temp != SWITCH_TASK_DARA) io_buffer_push(&tm_buffer_ctrl,temp);
		return 1;
	}
}

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
/**
 * @brief 将tss初始化成全0
 * 
 * @param tss 
 */
void init_TSS(struct TSS* tss){
	for(int i=0;i<26;i++,(int*)tss++)
		*(int*)tss=0;
	return;
}
/**
 * @brief 创建任务
 * 
 * @param funcaddr 函数地址
 * @param level 优先级
 * @param flags 标志
 * @return int 失败返回0，成功返回进程pid
 * @note 目前默认代码段cs:2<<3 地址0x101000；
 * @note 目前默认数据段及其他初代码段1<<3 地址0x0
 * @note 自动分配16k栈空间
 */
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
/**
 * @brief 注册任务
 * 
 * 
 * @param gs 段号
 * @param pid 任务id
 * @param limit 限长
 * @param settings 设置
 * @return 

 * @todo 搜索是否能分配任务结构体，填写相应结构体描述，返回pid
 */
void regtask(struct GDT_SEG* gs,int pid,unsigned int limit,unsigned int settings){
	set_gdt_segment(gs,limit,(unsigned int)&(prograsses[pid-1].tss),settings,0);
	prograsses[pid-1].statu = RUNABLE;
	multipc_ctrl.total_prograsses++;
	return;
}