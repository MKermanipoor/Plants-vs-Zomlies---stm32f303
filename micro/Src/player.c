#include "uartUtil.h"
#include "object.h"
#include "time.h"

int __score = 0;
char __hp = 5;
char __level = 1;

void set_level(char leve){
	__level = leve;
	set_zombie_move_time_by_level(leve);
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
void set_score(int s){
	__score = s;
}



void reset_hp(){
	__hp = 5;
}
char get_hp(){
	return __hp;
}

long loss_hp_time = 0;
void lost_hp(){
	if (__hp > 0 ){
		__hp --;
		loss_hp_time = getTime();
		U_set_life(__hp);
	}
}

void set_hp(char hp){
	__hp = hp;
	U_set_life(__hp);
}


void calculate_score(){
	__score += get_level() * (getTime() - get_game_time() + getTime() - loss_hp_time)/TIME_TO_SEC;
	U_score(__score);
}