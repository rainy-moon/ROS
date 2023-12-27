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
/**
 * @brief 返回两个数中大的数值
 * 
 * @param a1 
 * @param a2 
 * @return int 
 */
int max(int a1,int a2);
/**
 * @brief 返回两个数中小的数值
 * 
 * @param a1 
 * @param a2 
 * @return int 
 */
int min(int a1,int a2);
/**
 * @brief 用c字符分割字符串s为多个子串
 * 			注意：c不能为'\0'
 * 			开头和结尾若有分割字符则会被分割为空串
 * @param s 
 * @param c 
 * @return char** 
 */
unsigned char** splitby(unsigned char* s, char c);
/**
 * @brief 字符串深拷贝
 * 
 * @param s 拷贝目标串
 * @param ss 被拷贝串
 * @param length 截断长度，被拷贝串超出这个长度会被截断,保留长度为length-1
 */
void my_strcpy(char* s,const char* ss,int length);
/**
 * @brief tar是否在两数之间（包含两数）
 * 
 * @param tar 
 * @param a1 
 * @param a2 
 * @return int 在两数之间返回1，否则返回0
 */
int between(int tar,int a1,int a2);
#endif