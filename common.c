#include "common.h"

int strncmp(char *s1, char *s2, unsigned long long n){
	unsigned char is_equal = TRUE;
	
	unsigned long long i;
	for (i = 0; i < n; i++){
		if (*s1 != *s2){
			return 1;
		}
		if ((*s1 == '\0') | (*s2 == '\0')){
			return 1;
		}
		s1++;
		s2++;
	}
	if ((*s1 != '\0') | (*s2 != '\0')){
		return 1;
	}
	return 0;
}
