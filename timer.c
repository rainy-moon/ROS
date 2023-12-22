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
	int flag = store_eflags();
	cli();
	simlist_sortedinsert(&(tc.timelist),(struct node*)guard,0);
	tc.num=1;
	tc.time=0;
	load_eflags(flag);
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
	int flag = store_eflags();
	cli();
	t->timeout=time+tc.time;
	simlist_sortedinsert(&(tc.timelist),(struct node*)t,0);
	tc.num++;
	load_eflags(flag);
	return t->tid;
}
/**
 * @brief 删除计时器
 * 失败返回0，成功过返回1
 * @param tid 计时器的tid
 * @return int 
 * @note 注意会释放空间，不需要手动释放
 */
int timer_delete(int tid){
	int flag = store_eflags();
	cli();
	int index=simlist_find(&(tc.timelist),tid,3);
	if(index == -1) {load_eflags(flag); return 0;}
	int addr = (int)simlist_delete(&(tc.timelist),index);
	if(!addr) {load_eflags(flag); return 0;}
	tc.num--;
	load_eflags(flag);
	if(!mem_free((void*)addr,sizeof(struct timer))) return 0;
	return 1;
}
/**
 * @brief 重新设置计时器
 * 失败返回0，成功返回1
 * @param tid 
 * @param time 计时10ms*timeout
 * @param flags 
 * @param data 
 * @return int 
 */
int timer_resetall(int tid,unsigned int time,int flags,int data){
	int index=0;
	int flag = store_eflags();
	cli();
	struct timer* t=(struct timer*)(tc.timelist.head);
	for(;index<tc.num;index++){
		if(t->tid==tid) break;
		else t = t->next;
	}
	if(index >= tc.num){load_eflags(flag); return 0;}
	t->flags = flags;
	t->data = data;
	t->time = time;
	t->timeout = time+tc.time;
	simlist_resort(&(tc.timelist),index,0);
	load_eflags(flag);
	return 1;
}
/**
 * @brief 对于超时且循环的计时器重置超时时间
 * 
 * @param tid 
 * @return int 
 */
int timer_resettimeout(int tid){
	int index=0;
	int flag = store_eflags();
	cli();
	struct timer* t=(struct timer*)(tc.timelist.head);
	for(;index<tc.num;index++){
		if(t->tid==tid) break;
		else t = t->next;
	}
	if(index >= tc.num){load_eflags(flag); return 0;}
	t->timeout = t->time+tc.time;
	simlist_resort(&(tc.timelist),index,0);
	load_eflags(flag);
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
		if(!timer_resettimeout(((struct timer*)(tc.timelist.head))->tid))
			return 0;
	}
	else if(!timer_delete(((struct timer*)tc.timelist.head)->tid)) return 0;
	if(temp == SWITCH_TASK_DARA && multipc_ctrl.pr.size>0){
		// my_sprintf(s,"p %d %d",multipc_ctrl.pr.size,multipc_ctrl.pr.head->val[3]);
		// win_showsln(2,s,COLOR_BLACK);
		struct prograss* p = (struct prograss*)simlist_delete(&(multipc_ctrl.pr),0);
		simlist_sortedinsert(&(multipc_ctrl.pr),(struct node*)multipc_ctrl.pc,4); 
		// my_sprintf(s,"nextp %d %d",multipc_ctrl.pr.size,multipc_ctrl.pc->pid);
		// win_showsln(2,s,COLOR_BLACK);
		return p->pid+2;
	}else{
		if(temp != SWITCH_TASK_DARA) io_buffer_push(&tm_buffer_ctrl,temp);
		return 1;
	}
}

