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
void set_score(char s){
	__score = s;
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

void set_hp(char hp){
	__hp = hp;
	U_set_life(__hp);
}

char __level = 1;

void set_level(char leve){
	__level = leve;
}
char get_level(){
	return __level;
}

char __remainig_zombie = 4;
void set_remainig_zombie(char r_z){
	__remainig_zombie = r_z;
}
char get_remainig_zombie(){
	return __remainig_zombie;
}