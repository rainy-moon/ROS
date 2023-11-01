#ifndef _MEMORYH_
#define _MEMORYH_
#include "ros.h"
// struct define
// global define
// function define
int memcheck(int addrs,int addre);
void update_page_ctrl(int size,int start,int mode);
void* mem_malloc(int size);
void init_page_ctrl();
int mem_free(void* addr,int size);
#endif