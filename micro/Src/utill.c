#include "uartUtil.h"
#include <string.h>

double __rand[40];
char __end_rand = 40;
double __rand_now;

int getRand(int range){
	int result = ((int)(__rand[__end_rand] * range)) % range;
	__end_rand = (__end_rand + 39) % 40;
	return result;
}

void setRandomCount(int randomCount){
	double t = (double)randomCount / 128;
	if (__rand[__end_rand] != t){
		__end_rand = (__end_rand + 1) % 40;
		__rand[__end_rand] = t;
		__rand_now = __rand[__end_rand];
	}
}

char min(char a, char b){
	if (a < b)
		return a;
	return b;
}

char startWith(char str[], char pre[]){
	int index = 0;
	while (str[index] != '\0' && pre[index] != '\0' && pre[index] == str[index]){
		index ++;
	}
	
	return pre[index] == '\0';
}

void substr(char str[], char save[],int start, int lenght){
	int index;
	for (index=0 ; str[index + start] != '\0' && index<lenght ; index++){
		save[index] = str[index + start];
	}
	save[index] = '\0';
}

char get_boolean(char p){
	return getRand(1000) < p;
}