#include "uartUtil.h"
#include "state.h"
#include "stm32f3xx_hal.h"
#include "LiquidCrystal.h"

char state = SHOW_DEMO;



void set_state_without_uart(char s){
//	if (state != NEW_GAME && state != GAME && (s==GAME || s==NEW_GAME)){
//		noBlink();
//		clear
//	}
	
	state = s;
	
}

void set_state(char s){
	set_state_without_uart(s);
	U_send_state(state);
}

char get_state(){
	return state;
}