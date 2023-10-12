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
 * @brief 当头任务超时，入缓冲，
 * 判断设置如果是重复任务重新加入计时器
 * 删除计时器
 * @note 此函数在中断中执行
 * @return int 
 */
int timer_toc(){
	io_buffer_push(&tm_buffer_ctrl,(((struct timer*)(tc.timelist.head))->data));
	if((((struct timer*) tc.timelist.head)->flags&0x00000001) == 0){
		int time = ((struct timer*)tc.timelist.head)->time;
		int flags = ((struct timer*)tc.timelist.head)->flags;
		int data = ((struct timer*)tc.timelist.head)->data;
		return (timer_delete(((struct timer*)tc.timelist.head)->tid) && timer_malloc(time,flags,data));
	}
	else return timer_delete(((struct timer*)tc.timelist.head)->tid);
}

int init_multipc(){
	multipc.stt_tid = timer_malloc(2,0,1);
	init_simlist(&(multipc.PR));
	init_simlist(&(multipc.PS));
	init_simlist(&(multipc.PD));
	init_simlist(&(multipc.PZ));
	init_simlist(&(multipc.PN));
	
}