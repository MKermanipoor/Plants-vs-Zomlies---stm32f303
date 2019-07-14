#include "stm32f3xx_hal.h"
#include "uartUtil.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim8;


unsigned char led[11];
unsigned char light = 0;

void set_area_ligth(unsigned char l){
	if (l != light)
		U_ligth(l);
	light = l;
}

void refresh_led(){
	
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, light*led[1]);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, light*led[3]);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, light*led[5]);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, light*led[6]);
	
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, light*led[0]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, light*led[2]);
	
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, light*led[7]);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, light*led[4]);
	
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, light*led[8]);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, light*led[9]);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, light*led[10]);
}

void set_enable(char ledNumber, char state){
	if (ledNumber < 0 || ledNumber > 10)
		return;
	
	led[ledNumber] = state;
}