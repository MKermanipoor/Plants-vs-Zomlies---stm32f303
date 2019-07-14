#include "uartUtil.h"
#include "time.h"

unsigned long __time_m =0 ;

long getTime(){
	return __time_m;
}

void addTime(){
	__time_m ++;
	U_time(__time_m);
}

unsigned long __game_time = 0;

void set_game_time(long time){
	__game_time = time;
}

long get_game_time(){
	return __game_time;
}

long get_level_time(){
	return __game_time % LEVEL_TIME;
}