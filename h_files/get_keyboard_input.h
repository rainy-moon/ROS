#ifndef _GET_KEYBOARD_INPUTH_
#define _GET_KEYBOARD_INPUTH_
#include "ros.h"
// struct define
// global define 
struct io_buffer kb_buffer_ctrl;
unsigned char kb_buffer[64];
//按键编码对应表，不支持f11、f12、数字小键盘
static char dictionary[0x45]={
	0,0,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s',
	'd','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v',
	'b','n','m',',','.','/',0,'*',0,' ',0,0,0,0,0,0,
	0,0,0,0,0
};
// function define
void get_keyboard_input();
#endif