#include "uartUtil.h"


int __score = 0;

char __hp = 5;

int get_score(){
	return __score;
}
void add_score(char s){
	__score += s;
	U_score(__score);
}
void reset_score(){
	__score = 0;
	U_score(0);
}

void reset_hp(){
	__hp = 5;
}
char get_hp(){
	return __hp;
}

void lost_hp(){
	if (__hp > 0 ){
		__hp --;
		U_set_life(__hp);
	}
}