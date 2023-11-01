#ifndef _GET_KEYBOARD_INPUTH_
#define _GET_KEYBOARD_INPUTH_
#include "ros.h"
// struct define
// global define 
struct io_buffer kb_buffer_ctrl;
unsigned char kb_buffer[64];
// function define
void get_keyboard_input();
#endif