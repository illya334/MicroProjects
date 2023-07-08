// by Parfonov Illia

#include "includer.h"

unsigned int strlen(char *str){
	for(unsigned int i=0; ; i++)
		if(str[i]=='\0') return i;
}

bool cmpstr(char *str1, char *str2){
	if(str1 == null) return false;
	if(str2 == null) return false;

	unsigned int str1Len = strlen(str1);
	unsigned int str2Len = strlen(str2);

	if(str1Len > str2Len || str2Len > str1Len) return false;

	for(unsigned int i=0; i<str1Len; i++)
		if(str1[i] != str2[i]) return false;
	
	return true;
}

char *copystr(char *str){
	unsigned int len = strlen(str);
	char *buf = (char*)malloc( len );

	for(unsigned int i = 0; i < len; i++){ // copy
		buf[i] = str[i];
		buf[i+1] = 0;
	}

	return buf;
}
