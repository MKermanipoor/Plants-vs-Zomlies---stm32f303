#include "stm32f3xx_hal.h"

unsigned char __numOf_7seg [4];

void __numPrint(char num, char digit, char dot){
	switch(num){
		case 0:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);
			break;
		case 6:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);
			break;
		case 8:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 1);
			break;
		case 9:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 1);
			break;
	}
	
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, dot!=1);
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, digit==0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, digit==1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, digit==2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, digit==3);
}

void refresh_7seg(){
	__numPrint(__numOf_7seg[0], 0, 0);
	HAL_Delay(1);
	__numPrint(__numOf_7seg[1], 1, 1);
	HAL_Delay(1);
	__numPrint(__numOf_7seg[2], 2, 0);
	HAL_Delay(1);
	__numPrint(__numOf_7seg[3], 3, 0);
	HAL_Delay(1);
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
}

void numPrint(char num, char digit){
	__numOf_7seg[digit] = num;
}