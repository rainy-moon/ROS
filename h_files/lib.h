#ifndef _LIBH_
#define _LIBH_
#include "ros.h"
//部分c库简单复现 libc.c
/**
 * @brief ﹢整形转换字符串，注意s最好初始化15个字符
 * 
 * @param s 字符串预留内存位置
 * @param data 正整形
 */
void my_itos(char*s, int data);
/**
 * @brief 类似sprintf(s,"... %d",...)目前支持一个带替换参数，类型%d
 * 注意s初始化内存个数，避免溢出
 * @param s 
 * @param ss 
 * @param data 
 */
void my_sprintf(char* s,const char* ss,...);
int my_strlen(const char* s);
/**
 * @brief 比较字符串相同
 * 
 * @param s1 
 * @param s2 
 * @param length 比较位数
 * @return int 0相同，index+1第一个不相同的位置
 * @note 如果长度都比比较位数短且相同，则认为相同
 */
int my_strcmp(const char* s1,const char* s2,int length);
#endif