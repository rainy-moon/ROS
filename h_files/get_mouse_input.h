#ifndef _GET_MOUSE_INPUTH_
#define _GET_MOUSE_INPUTH_
#include "ros.h"
// struct define
// global define 
struct io_buffer ms_buffer_ctrl;
unsigned char ms_buffer[128];
// function define
int get_mouse_input(int ms_state);
#endif