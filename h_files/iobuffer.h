#ifndef _IOBUFFERH_
#define _IOBUFFERH_
#include "ros.h"
// struct define
/**
 * @brief io缓冲区
 * flags 0x1 空置位 0x2 溢出位
 * 		 0满，1有空位 1溢出
 */
struct io_buffer{
	unsigned char* buffer;
	int p,q,size;
	char flags;
};
// global define

// function define
void init_io_buffer(struct io_buffer* iobuf,int size,unsigned char* buffer);
int io_buffer_push(struct io_buffer* buffer,unsigned char data);
int io_buffer_pop(struct io_buffer* buffer);
int io_buffer_num(struct io_buffer* buffer);
#endif