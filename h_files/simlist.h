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
int simlist_insert(struct SIMLIST* sl,int index,struct node* newnode);
/**
 * @brief 删除链表节点
 * 返回删除节点的地址,错误返回0.注意返回的节点地址next指针会指向NULL
 * @param sl 
 * @param index 
 * @return struct node* 
 * @note 切记需要手动释放地址空间，删除节点不会自动释放
 */
struct node* simlist_delete(struct SIMLIST* sl,int index);
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
 * @param newnode
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
/**
 * @brief 深拷贝一个节点,注意next指针不会拷贝初始为NULL
 * 
 * @param n 节点地址
 * @return struct node* 
 * 正常深拷贝后的节点地址，失败返回NULL
 * 
 */
struct node* nodecpy(struct node* n);
//#question 是否要实现进程安全链表？
#endif