#include "string.h"

unsigned long strlen(const char* str){
	register const char *s;

	for (s = str; *s; ++s);
	return(s - str);
}