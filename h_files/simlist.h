#ifndef _SIMLISTH_
#define _SIMLISTH_
#include "ros.h"
// struct define
/**
 * @brief 模拟链表
 * @param size 链表能使用总长度
 * @param head 第一个节点地址
 */
struct SIMLIST{
	struct node* head;
	int size;
};
struct node{
	int val[255];
	struct node* next;
};
// global define
// function define
void init_simlist(struct SIMLIST* sl);
int simlist_insert(struct SIMLIST* sl,int index,struct node* new);
int simlist_delete(struct SIMLIST* sl,int index);
struct node* simlist_get(struct SIMLIST* sl,int index);
int simlist_free(struct SIMLIST* sl);
void simlist_sortedinsert(struct SIMLIST* sl,struct node* new,int index);

#endif