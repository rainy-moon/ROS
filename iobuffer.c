#include "ros.h"
void init_io_buffer(struct io_buffer* iobuf,int size,unsigned char* buffer){
	iobuf->p = 0;
	iobuf->q = 0;
	iobuf->flags = 0x1;
	iobuf->size = size;
	iobuf->buffer = buffer;
	return ;
}
//可能要考虑中断做成临界区？
int io_buffer_push(struct io_buffer* buffer,unsigned char data){
	int e = store_eflags();
	cli();
	if(buffer->p==buffer->q && !(buffer->flags & 0x1)){
		buffer->flags |= 0x2;
		load_eflags(e);
		return -1;
	}
	buffer->buffer[buffer->p] = data;
	buffer->p = (buffer->p+1)%buffer->size;
	if(buffer->p==buffer->q)
		buffer->flags &= 0xfe;
	load_eflags(e);
	return 0;
}
//可能要考虑中断做成临界区？
int io_buffer_pop(struct io_buffer* buffer){
	int e = store_eflags();
	cli();
	if(buffer->p==buffer->q && (buffer->flags & 0x1))
	{
		load_eflags(e);
		return -1;//
	}
	unsigned char data = buffer->buffer[buffer->q];
	buffer->q = (buffer->q+1)%buffer->size;
	buffer->flags |= 0x01;
	load_eflags(e);
	return (int)data;
}
int io_buffer_num(struct io_buffer* buffer){
	int e = store_eflags();
	cli();
	if(buffer->p<buffer->q){
		int temp = buffer->p+buffer->size-buffer->q;
		load_eflags(e);
		return temp;
	}
	else if (buffer->p==buffer->q && !(buffer->flags & 0x1)){
		load_eflags(e);
		return buffer->size;
	}
	else{
		load_eflags(e);
		return buffer->p-buffer->q;
	}
}