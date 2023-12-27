#include"ros.h"


void my_itos(char* s, int data) {
	int length = 0;
	if (!data){
		s[0] = '0';
		s[1] = '\0';
		return;
	}
	if(data<0){
		s[length]='-';
		length++;
		data = -data;
	}
	while (data) {
		int i = data % 10;
		s[length] = i + '0';
		data /= 10;
		length++;
	}
	int i,c;
	if(s[0]=='-'){
		c = (length-1) / 2;
		i = 1;
	}else{
		c = length / 2;
		i = 0;
	}
	for (; i < c; i++) {
		char swap = s[i];
		s[i] = s[length-1-i];
		s[length - 1 - i] = swap;
	}
	s[length] = '\0';
	return;
}


void my_sprintf(char* s, const char* ss, ...) {
	const char* sstemp = ss;
	char* stemp = s;
	int* ptr = (int*)&ss + 1;
	while (*sstemp) {
		if (*sstemp != '%') *(stemp++) = *(sstemp++);
		else {
			sstemp++;
			char concats[20];
			switch (*sstemp) {
				case 'd':
					my_itos(concats,*ptr);
					ptr++;
					break;
				case 's':
				{
					char* sptr = (char*)(*ptr);
					int i = 0;
					for (; *sptr; i++, sptr++)
						concats[i] = *sptr;
					concats[i] = '\0';
					ptr++;
					break;
				}
				case'c':
				{
					concats[0]=*((char*)ptr);
					concats[1]='\0';
					ptr++;
				}
			}
			for (int i=0; concats[i]; i++, stemp++)
				*stemp = concats[i];
			sstemp++;
		}
	}
	*stemp = '\0';
	return;
}

int my_strlen(const char* s){
	int length = 0;
	while(s[length])
		length++;
	return length;
}

int my_strcmp(const char* s1,const char* s2,int length){
	int i = 0;
	for(i = 0;i<length;i++){
		if(s1[i]!=s2[i]) return i+1;
		else if (!s1[i]) return 0;
		else continue;
	}
	return 0;
}

int max(int a1,int a2){
	if(a1>a2) return a1;
	else return a2;
}

int min(int a1,int a2){
	if(a1<a2) return a1;
	else return a2;
}

unsigned char** splitby(unsigned char* s,char c){
	ss[0] = (unsigned char*)1; 
	ss[1] = s;
	for(int i = 0;s[i];i++){
		if(s[i]==c){
			s[i] = '\0';
			ss[0] = (unsigned char*)((int)ss[0]+1);
			ss[(int)ss[0]] = s+i+1;
		}
	}
	return ss;
}

void my_strcpy(char* s,const char* ss,int length){
	int i = 0;
	for(i=0;i<length&&ss[i];i++){
		s[i] = ss[i];
	}
	if(i==length) s[i-1] = '\0';
	else s[i] = '\0';
}

int between(int tar,int a1,int a2){
	if(a1<a2 && tar>=a1 && tar<=a2) return 1;
	else if(a1>=a2 && tar<=a1 && tar>=a2) return 1;
	else return 0;
}