#define SHOW_DEMO 0
#define SHOW_MENU 1
#define SHOW_ABOUT 2
#define NEW_GAME 3
#define TEMP -1
#define GAME 4
#define GAME_OVER 5
#define SAVE_GAME 6

#include "uartUtil.h"

char state = SHOW_DEMO;

void set_state(char s){
	state = s;
	U_send_state(state);
}

void set_state_without_uart(char s){
	state = s;
}

char get_state(){
	return state;
}