#include"ros.h"
/**
 * @brief 初始化一个模拟链表,返回指针。分配失败返回0
 * @param struct SIMLIST* sl
 * @return struct SIMLIST* 
 */
void init_simlist(struct SIMLIST* sl){
	sl->size =  0;
	sl->head = (struct node*)NULL;
	return;
}
/**
 * @brief 指定位置插入链表
 * 
 * @param sl 
 * @param index 
 * @param value 
 * @return int
 */
int simlist_insert(struct SIMLIST* sl,int index,struct node*new){
	if(index>=sl->size) return 0;
	if(!new) return 0;
	//插在链表头
	struct node* p=sl->head;
	if(index==0){
		new->next=p;
		sl->head=new;
		sl->size++;
		return 1;
	}
	else{
		while(--index)
			p=p->next;
		new->next=p->next;
		p->next=new;
		sl->size++;
		return 1;
	}
}
/**
 * @brief 删除链表节点
 * 返回0删除失败，成功返回1
 * @param sl 
 * @param index 
 * @return int 
 */
int simlist_delete(struct SIMLIST* sl,int index){
	if(index>=sl->size) return 0;
	struct node* p = sl->head;
	//删头
	if(index==0){
		sl->head = sl->head->next;
		sl->size--;
		if(!mem_free(p,sizeof(struct node))) return 0;
		return 1;
	}
	else {
		while(--index)
			p=p->next;
		struct node* temp = p->next;
		p->next = temp->next;
		sl->size--;
		if(!mem_free(temp,sizeof(struct node))) return 0;
		return 1;
	}
}
/**
 * @brief 返回链表下标对应节点
 * 
 * @param sl 
 * @param index 
 * @return struct node* 
 */
struct node* simlist_get(struct SIMLIST* sl,int index){
	if(index<0||index>=sl->size) return NULL;
	else{
		struct node* p = sl->head;
		while(index--)
			p=p->next;
		return p;
	}
}

/**
 * @brief 释放链表空间
 * 返回1正常释放 0异常
 * @param sl 
 * @return struct SIMLIST*
 */
int simlist_free(struct SIMLIST* sl){
	if(sl->size==0) return 0;
	struct node* p1 = sl->head;
	struct node* p2 = p1->next;
	for(int i=0;i<sl->size;i++){
		while(p1){
			if(!mem_free(p1,sizeof(struct node))) return 0;
			p1=p2;
			p2=p1->next;
		}
	}
	sl->size = 0;
	sl->head = (struct node*) NULL;
	return 1;
}
/**
 * @brief 维护从小到大链表的插入,稳定排序
 * 
 * @param sl 
 * @param new 
 * @param index 大小比较时使用value值的第几个值（0-254）
 * @return 
 */
void simlist_sortedinsert(struct SIMLIST* sl,struct node* new,int index){
	struct node* p = sl->head;
	if(!p || p->val[index]>new->val[index]){
		//插头
		new->next=p;
		sl->head=new;
		sl->size++;
		return ;
	}
	while(p->next && p->next->val[index]<=new->val[index])
		p=p->next;
	new->next=p->next;
	p->next=new;
	sl->size++;
	return ;
}