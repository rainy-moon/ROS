#include"ros.h"
int mem_scanner = 0;
int memcheck(int addrs,int addre){
	for(int i=addrs;i<=addre;i+=0x2000){
		int test0 = 0xaa55aa55,test1 = 0x55aa55aa;
		int *p = (int*)(i+0x1ffc);
		int temp = *(p);
		*p = test0;
		*p ^= 0xffffffff;
		if(*p!=test1){
			*p = temp;
			return (i-addrs)/1024;
		}
		else{
			*p ^= 0xffffffff;
			if(*p!=test0){
				*p=temp;
				return (i-addrs)/1024;
			}
		}
	}
	return (addre-addrs)/1024;
}

void init_page_ctrl(){
	for(int* i=(int*)0x400000;i<=(int*)0x40ffff;i++)
		*i=0;
	return;
}
/**
 * @brief 页分配内存，每次至少1kb（一页），不足会补足
 * 失败返回0 成功返回地址
 * @param size 
 * @return void* 
 */
void* mem_malloc(int size){
	size = (size+0x3ff)/0x400;
	int length=0;
	for(int i=0;i< 64*1024*8;i++){
		if((*((char*)((i/8)+0x400000))&(0x1<<(i%8)))==0)
		{
			length++;
			if(length>=size) 
			{
				
				void* res = (void*)(0x520000+mem_scanner*0x400);
				update_page_ctrl(size,mem_scanner,1);
				mem_scanner = 0;
				
				return (void*)res;
			}
		}
		else{
			length = 0;
			mem_scanner = i+1;
		}
	}
	mem_scanner=0;
	return (void*)0;
} 
/**
 * @brief 释放内存
 * 
 * @param addr 
 * @param size 
 * @return int 1正常释放，0释放异常
 */
int mem_free(void* addr,int size){
	size = (size+0x3ff)/0x400;
	int pgnum = ((int)addr-0x520000)/0x400;
	if(size<=0||size>512*1024||pgnum<0 || pgnum>=64*1024*8) return 0;
	update_page_ctrl(size,pgnum,0);
	return 1;
}
/**
 * @brief 更新页表
 * 
 * @param size 更新几页
 * @param start 开始页
 * @param mode 1标记为使用，0标记为空
 */
void update_page_ctrl(int size,int start,int mode){
	for(int i=start;i<start+size;i++){
		char* temp = (char*) ((i/8)+0x400000);
		if(mode) *temp |= (0x1<<(i%8));
		else *temp &= ~(0x1<<(i%8)); 
	}
	return;
}