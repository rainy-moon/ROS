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
/**
 * @brief 初始化一个模拟链表
 * @param struct SIMLIST* sl
 * @return struct SIMLIST* 
 */
void init_simlist(struct SIMLIST* sl);
/**
 * @brief 指定位置插入链表
 * 
 * @param sl 
 * @param index 
 * @param value 
 * @return int
 */
int simlist_insert(struct SIMLIST* sl,int index,struct node* new);
/**
 * @brief 删除链表节点
 * 返回0删除失败，成功返回1
 * @param sl 
 * @param index 
 * @return int 
 */
int simlist_delete(struct SIMLIST* sl,int index);
/**
 * @brief 返回链表下标对应节点
 * 
 * @param sl 
 * @param index 
 * @return struct node* 
 */
struct node* simlist_get(struct SIMLIST* sl,int index);
/**
 * @brief 释放链表空间
 * 返回1正常释放 0异常
 * @param sl 
 * @return struct SIMLIST*
 */
int simlist_free(struct SIMLIST* sl);
/**
 * @brief 维护从小到大链表的插入,稳定排序
 * 
 * @param sl 
 * @param new 
 * @param index 大小比较时使用value值的第几个值（0-254）
 * @return 
 */
void simlist_sortedinsert(struct SIMLIST* sl,struct node* newnode,int index);
/**
 * @brief 查询链表中第index处的值=value的第一个节点
 * 未找到返回-1，找到返回下标
 * @param sl 
 * @param value 
 * @param index 
 * @return int 
 */
int simlist_find(struct SIMLIST* sl,int value,int index);
#endif