#ifndef _GET_TIMER_INPUTH_
#define _GET_TIMER_INPUTH_
#include "ros.h"
// struct define
// global define 
struct io_buffer tm_buffer_ctrl;
unsigned char tm_buffer[128];
// function define
void get_timer_input();
#endif