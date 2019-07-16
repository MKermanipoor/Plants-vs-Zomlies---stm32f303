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
#include "utill.h"
#include "7segment.h"
#include "object.h"
#include "LCDutill.h"
#include "LED.h"
#include "player.h"
#include "keypadUtil.h"
#include "uartUtil.h"
#include <string.h>
#include "state.h"
#include "time.h"

extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim1;

//timer 2 variable

char initFlag = 0;

long __last_time_zombie_created = 0;
long __start_save_game_time;

char name[20] = "\0";
char temp_name[20] ="\0";

char clear_showDemo = 0;
void showDemo(unsigned long time){
	
	int devide = (time)*10/TIME_TO_SEC;
	
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
		set_state(SHOW_MENU);
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

char __init_show_menu = 0;
char __init_show_about = 0;
void show_menu(){
	if(!__init_show_menu){
		clear();
		setCursor(1,0);
		print("New Game");
		setCursor(1,1);
		print("Load Game");
		setCursor(1,2);
		print("About");
		blink();
		__init_show_menu = 1;
		__init_show_about = 0;
	}
}

void show_about(){
	if (!__init_show_about){
		noBlink();
		clear();
		setCursor(0,0);
		print("Fuck sedaghat!!");
		__init_show_about = 1;
		__init_show_menu = 0;
	}
}


char __game_over_init = 0;
void show_game_over(){
	if (!__game_over_init){
		setCursor(3,1);
		print("GAME OVER");
		setCursor(3,2);
		print("MOTHER FUCKER");
		__game_over_init = 1;
	}
}

char __you_win_init = 0;
void show_you_win(){
	if (!__you_win_init){
		clear();
		clear_lcd();
		disable_blink();
		setCursor(0,0);
		print("YOU WIN");
		__you_win_init = 1;
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
	
	if(get_state() == SHOW_MENU){
		switch(get_blink_row()){
			case 0:
				clear();
				HAL_Delay(CLEAR_DALAY);
				noBlink();
				enable_blink();
				set_state(NEW_GAME);
				break;
			case 2:
				set_state(SHOW_ABOUT);
				break;
		}
	}else if(get_state() == SHOW_ABOUT){
		set_state(SHOW_MENU);
	}
	
  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
* @brief This function handles EXTI line3 interrupt.
*/
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */
	for (char i=0; i<4; i++){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, i==0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, i==1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, i==2);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, i==3);
		
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3)){
			switch(i){			
				case 1:
					if (get_state() == SHOW_MENU){
						set_blink_row((get_blink_row() + 2) % 3);
					}else if (get_state() == GAME || get_state() == NEW_GAME){
						add_blink_row(3);
					}
	
					break;
				case 3:
					if (get_state() == GAME){
						__start_save_game_time = getTime();
						disable_blink();
						clear();
						HAL_Delay(CLEAR_DALAY);
						setCursor(0,1);
						print("Enter your name:");
						setCursor(0,2);
						blink();
						reset_keypad();
						set_state(SAVE_GAME);
					}
					break;
			}
			if (get_state() == SAVE_GAME){
				press_keypad(0,i);
			}
			break;
		}
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 1);
  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */
	
  /* USER CODE END EXTI3_IRQn 1 */
}

/**
* @brief This function handles EXTI line4 interrupt.
*/
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */
	for (char i=0; i<4; i++){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, i==0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, i==1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, i==2);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, i==3);
		
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)){
			switch(i){
				case 1:
					if (get_state() == SHOW_MENU){
						set_blink_row((get_blink_row() + 1) % 3);
					}else if (get_state() == GAME || get_state() == NEW_GAME){
						add_blink_row(1);
					}
	
					break;
			}
			
			if (get_state() == SAVE_GAME){
				press_keypad(1,i);
			}
			break;
		}
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 1);
  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
* @brief This function handles ADC1 and ADC2 interrupts.
*/
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */
	set_area_ligth(HAL_ADC_GetValue(&hadc1)*100/63);
	refresh_led();
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
	
	for (char i=0; i<4; i++){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, i==0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, i==1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, i==2);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, i==3);
		
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5)){
			switch(i){
				case 0:
					break;
			}
			if (get_state() == SAVE_GAME){
				press_keypad(2,i);
			}
			break;
		}else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)){
			switch(i){
				case 1:
					if (get_state() == GAME || get_state() == NEW_GAME){
						create_plant(1, get_blink_row(), get_blink_column()); 
					}
					break;
				case 2:
					if (get_state() == GAME || get_state() == NEW_GAME){
						create_plant(2, get_blink_row(), get_blink_column()); 
					}
					break;
				case 3:
					if (get_state() == GAME || get_state() == NEW_GAME){
						create_plant(3, get_blink_row(), get_blink_column()); 
					}else if (get_state() == SAVE_GAME){
						noBlink();
						clear();
						HAL_Delay(CLEAR_DALAY);
						set_state(GAME);
						
						setTime(__start_save_game_time);
						U_save_game(get_game_time(), get_level_time(), get_level());
						enable_blink();
					}
					break;
			}
			
			break;
		}
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 1);
  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
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
		
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
		
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
		
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
		
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
		
		initFlag = 1;
		
		clear_lcd();
	}
	
	
	switch(get_state()){
		case SHOW_DEMO:
			showDemo(getTime());
			break;
		case SHOW_MENU:
			show_menu();
			setCursor(0,get_blink_row());
			break;
		case SHOW_ABOUT:
			show_about();
			break;
		case NEW_GAME:
			if (get_plant_size() >= 3){
				set_state(GAME);
				enable_wink();
				start_plant_timer();
				set_game_time(getTime());
				set_level_time(getTime());
			}
			numPrint(get_hp(), 0);
			print_all_plant();
			break;
		case GAME:
			// end of game
			if (get_level() > 8){
				disable_blink();
				__init_show_about = 0;
				__init_show_menu = 0;
				clear_lcd();
				set_state(WIN);
				break;
			}
			// level up and reset remainig zombie count
			if (getTime() - get_level_time() > LEVEL_TIME){
				set_level(get_level() + 1);
				U_round(get_level());
				set_level_time(getTime());
				set_remainig_zombie(get_remainig_zombie() + 2 + get_level() * 2);
			}
			
			// create zombie
			if (get_remainig_zombie() > 0 && getTime() - __last_time_zombie_created > TIME_TO_SEC * 12/5 && get_zombie_size() < 5){
				struct Zombie z_t = create_zombie(getRand(4));
				if (z_t.id != -1){
					set_remainig_zombie(get_remainig_zombie() -1);
					__last_time_zombie_created = getTime();
				}
			}
			
			// create bonus
			if (get_boolean(1)){
				create_bonus();
			}
			
			
			
			move_all_zombies();
			check_bonus();
			
			//print hp
			numPrint(get_hp(), 0);
			
			print_all_zombies();
			print_all_plant();
			print_bonus();
			if (get_hp() < 1){
				disable_blink();
				clear();
				__game_over_init = 0;
				set_state(GAME_OVER);
				show_game_over();
			}
			break;
			
		case SAVE_GAME:
				get_name(temp_name);
				if (strcmp(name , temp_name) != 0){
					noBlink();
					setCursor(0,2);
					get_name(name);
					print(name);
					blink();
				}
			break;
		case WIN:
			show_you_win();
	}
	show_blink();
	refresh_wink();
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
	addTime();
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
	if (get_state() == GAME){
		long t = (getTime() - get_game_time())/TIME_TO_SEC;
	
		numPrint(t%10,1);
		t/=10;
		numPrint(t%10,2);
		t/=10;
		numPrint(t%10,3);
	}
	refresh_7seg();
	
	if(get_state() == GAME || get_state() == NEW_GAME){
		set_enable(0, get_level() >= 1);
		set_enable(1, get_level() >= 2);
		set_enable(2, get_level() >= 3);
		set_enable(3, get_level() >= 4);
		set_enable(4, get_level() >= 5);
		set_enable(5, get_level() >= 6);
		set_enable(6, get_level() >= 7);
		set_enable(7, get_level() >= 8);
		
		set_enable(8, get_plant_enable(1));
		set_enable(9, get_plant_enable(2));
		set_enable(10, get_plant_enable(3));
	}
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
	if(is_blink_enable()){
		char t = HAL_ADC_GetValue(&hadc3) * 19 / 63;
		
		set_blink_column(t);
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
	setRandomCount(HAL_ADC_GetValue(&hadc4));
  /* USER CODE END ADC4_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc4);
  /* USER CODE BEGIN ADC4_IRQn 1 */
	HAL_ADC_Start_IT(&hadc4);
  /* USER CODE END ADC4_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
