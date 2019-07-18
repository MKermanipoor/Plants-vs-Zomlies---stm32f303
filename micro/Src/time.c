#include "uartUtil.h"

unsigned long __time_m =0 ;

long getTime(){
	return __time_m;
}

void addTime(){
	__time_m ++;
	U_time(__time_m);
}

void setTime(long time){
	__time_m = time;
}

unsigned long __game_time = 0;

void set_game_time(long time){
	__game_time = time;
	U_game_time(time);
}

long get_game_time(){
	return __game_time;
}

long __level_time = 0;
long get_level_time(){
	return __level_time;
}

void set_level_time(long level_time){
	__level_time = level_time;
}