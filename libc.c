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
					break;
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