/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"
#include "stm32f3xx_it.h"

/* USER CODE BEGIN 0 */
#include "LiquidCrystal.h"
#include <string.h>
#define TIME_TO_SEC 1000

int getRand(int);


char min(char a, char b){
	if (a < b)
		return a;
	return b;
}

//timer 2 variable
const char SHOW_DEMO = 0;
const char TEMP = -1;

char initFlag = 0;
char state = TEMP;

//timer 3 variable
unsigned long __time_m = 0;

//timer 4 variable
unsigned char __numOf_7seg [4];



//ADC 4 variable
double __rand = 0;
int __rand_count = 0;

//ADC 3 variable
unsigned char lcd [20][4];
unsigned char __input_row_blink = 0;
unsigned char __input_column_blink = 0;
unsigned char row_blink = 0;
unsigned char column_blink = 0;
unsigned char last_row_blink = 0;
unsigned char last_column_blink = 0;
char blinkEnable = 1;
char blinkChange = 0;
unsigned char __now_blink_enable = 0;
long __last_change_blink = 0;
const long timeToBlink = TIME_TO_SEC / 6;


void write_lcd(char c, char column, char row){
	lcd[column][row] = c;
	if(blinkEnable || column != column_blink || row != row_blink){
		setCursor(column, row);
		write(c);
	}
}

void show_blink(){
	if(blinkChange){
		setCursor(last_column_blink, last_row_blink);
		write(lcd[last_column_blink][last_row_blink]);
		blinkChange = 0;
	}
	
	if(blinkEnable){
		column_blink = __input_column_blink;
		row_blink = __input_row_blink;
		if(__time_m - __last_change_blink > timeToBlink){
			__now_blink_enable = ! __now_blink_enable;
			if(__now_blink_enable){
				setCursor(column_blink, row_blink);
				write(95);
				last_column_blink = column_blink;
				last_row_blink = row_blink;
			}else{
				setCursor(column_blink, row_blink);
				write(lcd[column_blink][row_blink]);
			}
			__last_change_blink = __time_m;
		}
	}else{
		__now_blink_enable = 0;
	}
	
	
}

//palnt data
struct Plant{
	unsigned char row;
	unsigned char column;
	unsigned char kind;
	unsigned char hp;
	unsigned char id;
};

unsigned char __plant_size = 0;
struct Plant __plants [10];

struct Plant getPlant(char kind, char row, char column){
	struct Plant result;
	result.column = column;
	result.row = row;
	result.kind = kind;
	switch(kind){
		case 1:
		case 2:
		case 3:
			result.hp = kind;
			break;
		default:
			result.kind = 1;
			result.hp = 1;
			break;
	}
	
	if(__plant_size < 10){
		for (char i=0 ; i<__plant_size ; i++){
			if (__plants[i].column == column && __plants[i].row == row){
				return result;
			}
		}
		result.id = __plant_size;
		__plants[__plant_size] = result;
		__plant_size++;
	}
	
	
	return result;
}

void remove_plant(struct Plant plant){
	if (plant.id < 0)
		return;
	
	if (plant.id > __plant_size)
		return;
	
	__plant_size --;
	__plants[plant.id] = __plants[__plant_size];
	__plants[plant.id].id = plant.id;
	write_lcd(32, plant.column, plant.row);
}

void print_plant(struct Plant plant){
	write_lcd(plant.kind + 3, plant.column,plant.row);
}

void print_all_plant(){
	for(char i=0 ; i<__plant_size ; i++){
		print_plant(__plants[i]);
	}
}

//zombies data
struct Zombie{
	long lastTimeMove;
	unsigned char row;
	unsigned char column;
	unsigned char before_row;
	unsigned char before_column;
	unsigned char kind;
	unsigned char hp;
	unsigned char id;
};

struct Zombie __zombies [10];
unsigned char __zombies_size = 0;

unsigned int speed_time = 1.5 * TIME_TO_SEC;

struct Zombie getZombie(char kind){
	struct Zombie result;
	result.column = getRand(20);
	result.row = 0;
	result.before_column = 0;
	result.before_row = 0;
	result.kind = kind;
	result.id = -1;
	
	switch(kind){
		case 1:
		case 2:
		case 3:
		case 4:
			result.hp = kind;
			break;
		default:
			result.hp = 1;
			result.kind = 1;
	}
	
	result.lastTimeMove = __time_m;
	
	for(int i=0; i<__zombies_size; i++){
		if(result.column == __zombies[i].column && result.row == __zombies[i].row)
			return result;
	}
	
	if(__zombies_size < 10){
		result.id = __zombies_size;
		__zombies[__zombies_size] = result;
		__zombies_size++;
	}
	return result;
}

void removeZombie(struct Zombie z){
	if (z.id < 0)
		return;
	
	if (z.id > __zombies_size)
		return;
	
	__zombies_size --;
	__zombies[z.id] = __zombies[__zombies_size];
	__zombies[z.id].id = z.id;
	write_lcd(32, z.before_column, z.before_row);
}

void print_zombie(struct Zombie z){
	if(z.before_row != z.row){
		write_lcd(32, z.before_column, z.before_row);
	}
	write_lcd(z.kind-1, z.column, z.row);
}

void print_all_zombies(){
	for (int i=0; i<__zombies_size ; i++)
		print_zombie(__zombies[i]);
}

void move_zombie(struct Zombie *z){
	if(__time_m - z->lastTimeMove > speed_time){
		z->row++;
		z->lastTimeMove = __time_m;
		
		for (char i=0; i<__plant_size; i++){
			if(__plants[i].column == z->column && __plants[i].row == z->row){
				char temp = min(__plants[i].hp, z->hp);
				__plants[i].hp -= temp;
				if(__plants[i].hp <= 0)
					remove_plant(__plants[i]);
				
				z->hp -= temp;
				if(z->hp <= 0)
						removeZombie(*z);
				
			}
		}
		
		if(z->row >= 4){
			removeZombie(*z);
		}
	}else{
		z->before_column = z->column;
		z->before_row = z->row;
	}
}

void move_all_zombies(){
	for (int i=0;i<__zombies_size;i++)
		move_zombie(&__zombies[i]);
}


int getRand(int range){
	return ((int)(__rand * range)) % range;
}

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

void numPrint(char num, char digit){
	__numOf_7seg[digit] = num;
}









char clear_showDemo = 0;
void showDemo(unsigned long time){
	
	int devide = (time)/TIME_TO_SEC;
	
	if(devide <= 10){
		numPrint(10 - devide,0);
		numPrint(10 - devide,1);
		numPrint(10 - devide,2);
		numPrint(10 - devide,3);
	}
	
	if(devide>10){
		if(clear_showDemo)
			return;
		
		
		for (int i=0 ; i<4; i++){
			setCursor(0,i);
			write(32);
			setCursor(19,i);
			write(32);
		}
		state = TEMP;
		clear_showDemo = 1;
		return;
	}else if(devide == 0){
		for (int i=0 ; i<4; i++){
			setCursor(0,i);
			for (int j=0; j<20 ; j++){
				write(255);
			}
		}
	}else if (devide == 1){
		for (int i=0 ; i<4; i++){
			setCursor(9,i);
			write(41);
			write(40);
		}
	}else{
		for (int i=0 ; i<4; i++){
			setCursor(10-devide,i);
			write(41);
			write(32);
			setCursor(8+devide,i);
			write(32);
			write(40);
		}
	}
	
}


/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart2;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
	
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */
	__rand_count = (HAL_ADC_GetValue(&hadc4));
	__rand = (double)__rand_count / 59;
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line0 interrupt.
*/
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
	//if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0))
	getPlant(1, row_blink, column_blink);
  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
* @brief This function handles EXTI line1 interrupt.
*/
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
	//if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1))
		getPlant(2, row_blink, column_blink);
  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
* @brief This function handles EXTI line2 and Touch Sense controller.
*/
void EXTI2_TSC_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_TSC_IRQn 0 */
	//if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2))
	getPlant(3, row_blink, column_blink);
  /* USER CODE END EXTI2_TSC_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_TSC_IRQn 1 */

  /* USER CODE END EXTI2_TSC_IRQn 1 */
}

/**
* @brief This function handles ADC1 and ADC2 interrupts.
*/
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */
	
  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */
	HAL_ADC_Start_IT(&hadc1);
  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
* @brief This function handles EXTI line[9:5] interrupts.
*/
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)){
		blinkChange=1;
			__input_row_blink+=3;
			__input_row_blink=__input_row_blink%4;
	
	}
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)){
		blinkChange=1;
		__input_row_blink++;
		__input_row_blink=__input_row_blink%4;
		
	}
  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
* @brief This function handles TIM2 global interrupt.
*/
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	if(!initFlag){
		HAL_ADC_Start_IT(&hadc1);
		HAL_ADC_Start_IT(&hadc3);
		HAL_ADC_Start_IT(&hadc4);
		HAL_TIM_Base_Start_IT(&htim3);
		HAL_TIM_Base_Start_IT(&htim4);
		initFlag = 1;
		
		for (int i=0 ; i<20 ; i++)
			for (int j=0 ; j<4 ; j++)
				lcd[i][j] = 32;
			
	}
	
	
	switch(state){
		case SHOW_DEMO:
			showDemo(__time_m);
			break;
		case TEMP:
			getZombie(getRand(4)+1);
			move_all_zombies();
			print_all_zombies();
			print_all_plant();
			break;
	}
	show_blink();
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
	HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END TIM2_IRQn 1 */
}

/**
* @brief This function handles TIM3 global interrupt.
*/
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	__time_m++;
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
	HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END TIM3_IRQn 1 */
}

/**
* @brief This function handles TIM4 global interrupt.
*/
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
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
	
	
  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */
	HAL_TIM_Base_Start_IT(&htim4);
  /* USER CODE END TIM4_IRQn 1 */
}

/**
* @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
*/
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
* @brief This function handles ADC3 global interrupt.
*/
void ADC3_IRQHandler(void)
{
  /* USER CODE BEGIN ADC3_IRQn 0 */
	if(blinkEnable){
		char t = HAL_ADC_GetValue(&hadc3) * 19 / 63;
		blinkChange = (t!=column_blink)||blinkChange;
		__input_column_blink = t;
	}
  /* USER CODE END ADC3_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc3);
  /* USER CODE BEGIN ADC3_IRQn 1 */
	HAL_ADC_Start_IT(&hadc3);
  /* USER CODE END ADC3_IRQn 1 */
}

/**
* @brief This function handles ADC4 interrupt.
*/
void ADC4_IRQHandler(void)
{
  /* USER CODE BEGIN ADC4_IRQn 0 */
	__rand_count = (HAL_ADC_GetValue(&hadc4));
	__rand = (double)__rand_count / 59;
  /* USER CODE END ADC4_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc4);
  /* USER CODE BEGIN ADC4_IRQn 1 */
	HAL_ADC_Start_IT(&hadc4);
  /* USER CODE END ADC4_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
