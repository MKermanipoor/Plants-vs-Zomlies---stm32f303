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
#include <string.h>


//timer 2 variable
const char SHOW_DEMO = 0;
const char SHOW_MENU = 1;	
const char SHOW_ABOUT = 2;
const char TEMP = -1;

char initFlag = 0;
char state = SHOW_DEMO;


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
		state = SHOW_MENU;
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
	/**** plant event plant 1
	getPlant(1, row_blink, column_blink);
	*****/
	
	if(state == SHOW_MENU){
		switch(get_blink_row()){
			case 0:
				clear();
				noBlink();
				enable_blink();
				state = TEMP;
				break;
			case 2:
				state = SHOW_ABOUT;
				break;
		}
	}else if(state == SHOW_ABOUT){
		state = SHOW_MENU;
	}
	
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
	create_plant(2, get_blink_row(), get_blink_column());
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
	create_plant(3, get_blink_row(), get_blink_column());
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
		set_blink_change(1);
		if (is_blink_enable()){
			add_blink_row(3);
			
		}else{
			set_blink_row( (get_blink_row() +  2) % 3);
		}
	
	}
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)){
		set_blink_change(1);
		if (is_blink_enable()){
			add_blink_row(1);;
		}else{
			set_blink_row( (get_blink_row() +  1) % 3);
		}
		
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
		
		clear_lcd();
	}
	
	
	switch(state){
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
		case TEMP:
			create_zombie(getRand(4)+1);
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
	refresh_7seg();
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
		set_blink_change((t!=get_blink_column())||is_blick_change());
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
