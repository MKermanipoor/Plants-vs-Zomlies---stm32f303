#include "object.h"
#include "keypadUtil.h"
#include "player.h"
#include "utill.h"
#include "stm32f3xx_hal.h"
#include "state.h"
#include "time.h"
#include "LiquidCrystal.h"
#include "LCDutill.h"
#include <string.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart2;

#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
	
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 1000); // change &uart1 accordingly
	return ch;
}


//zombie
void U_create_zombie(struct Zombie z){
	printf("zc:%d,%d,%d\n", z.kind, z.row, z.column);
}

void U_remove_zombie(struct Zombie z){
	printf("zr:%d,%d\n", z.row-1, z.column);
}

void U_move_zombie(struct Zombie z){
	printf("zm:%d,%d,%d,%d\n",  z.before_row, z.before_column,z.row,z.column);
}

//plant
void U_create_plant(struct Plant p){
	printf("pc:%d,%d,%d\n", p.kind , p.row, p.column);
}
void U_remove_plant(struct Plant p){
	printf("pr:%d,%d\n",p.row, p.column);
}
void U_enable_plant(char kind, char enable){
	printf("pe:%d,%d", kind, enable);
}

//bouns
void U_bouns_create(struct Bonus b){
	//printf("bc:%d,%d,%d\n", b.kind, b.row, b.column);
}

void U_bouns_remove(struct Bonus b){
	//printf("br:%d,%d\n", b.row, b.column);
}

//ligth
long last_time_send_ligth = 0;
void U_ligth(char light){
//	if (getTime() - last_time_send_ligth > 10 * TIME_TO_SEC){
//		printf("l:%d\n", light);
//		last_time_send_ligth = getTime();
//	}
}

//score
void U_score(int score){
	printf("s:%d\n", score);
}

//round
void U_round(char round){
	printf("r:%d\n", round);
}

//time
long last_time_send_time = 0;
void U_time(long time){
	if (getTime() - last_time_send_time > 1 * TIME_TO_SEC){
		printf("t:%ld\n", time);
		last_time_send_time = getTime();
	}
}

//life
void U_set_life(char life){
	printf("ls:%d\n", life);
}

//save game
void U_save_game(long game_time, long level_time, char level){
	printf("save:");
	
	
	char name[20];
	get_name(name);
	printf("%s,", name); 
	
	//plant
	
	printf("p:%d,", get_plant_size());
	for (char i=0; i<get_plant_size(); i++){
		struct Plant p = *get_plant(i);
		printf("%d,%d,%d,%d,", p.kind, p.row, p.column, p.hp);
	}
	
	//plant time
	printf("p_t:%ld,%ld,%ld,", get_last_use_plant_time(0), get_last_use_plant_time(1), get_last_use_plant_time(2));
	
	//zombie
	printf("z:%d,", get_zombie_size());
	for (char i=0 ; i<get_zombie_size() ; i++){
		struct Zombie z = get_zombie(i);
		printf("%d,%d,%d,%d,%ld", z.kind, z.row, z.column, z.hp, z.lastTimeMove);
	}
	/// todo ramainig zombie in this level
	
	// remaning hp
	printf("hp:%d,", get_hp());
	
	// start geme time and time
	printf("g_t:%ld,t:%ld,", get_game_time(), getTime());
	
	//score
	printf("s:%d", get_score());
}

void U_send_state(char s){
	char state[10];
	
	switch (s){
		case SHOW_DEMO:
		case SHOW_MENU:
			strcpy(state, "menu");
			break;
		case NEW_GAME:
		case GAME:
			strcpy(state, "game");
			break;
		case SHOW_ABOUT:
			strcpy(state, "about");
			break;
		case GAME_OVER:
			strcpy(state, "game_over");
			break;
		case SAVE_GAME:
			strcpy(state, "save_game");
			break;
	}
	
	printf("state:%s,\n", state);
}

void load(char data[]){
	char temp[10];
	
	int start_index = 2;
	int text_size = 0;
	
	//plant
	for (text_size = 0; data[start_index + text_size] != ',' || data[start_index + text_size] != '\0';text_size++);
	substr(data, temp, start_index, text_size);
	char plant_size = atoi(temp);
	start_index += text_size + 1;
	for (char i=0; i<plant_size; i++){
		for (text_size = 0; data[start_index + text_size] != ',' || data[start_index + text_size] != '\0';text_size++);
		substr(data, temp, start_index, text_size);
		char plant_kind = atoi(temp);
		start_index += text_size + 1;
		
		for (text_size = 0; data[start_index + text_size] != ',' || data[start_index + text_size] != '\0';text_size++);
		substr(data, temp, start_index, text_size);
		char plant_row = atoi(temp);
		start_index += text_size + 1;
		
		for (text_size = 0; data[start_index + text_size] != ',' || data[start_index + text_size] != '\0';text_size++);
		substr(data, temp, start_index, text_size);
		char plant_column = atoi(temp);
		start_index += text_size + 1;
		
		for (text_size = 0; data[start_index + text_size] != ',' || data[start_index + text_size] != '\0';text_size++);
		substr(data, temp, start_index, text_size);
		char plant_hp = atoi(temp);
		start_index += text_size + 1;
		
		create_plant_with_hp(plant_kind, plant_row, plant_column, plant_hp);
	}
	
	
	//plant time
	start_index += 4;
	
	for (text_size = 0; data[start_index + text_size] != ',' || data[start_index + text_size] != '\0';text_size++);
	substr(data, temp, start_index, text_size);
	long plant_0_time = atoi(temp);
	start_index += text_size + 1;
	
	for (text_size = 0; data[start_index + text_size] != ',' || data[start_index + text_size] != '\0';text_size++);
	substr(data, temp, start_index, text_size);
	long plant_1_time = atoi(temp);
	start_index += text_size + 1;
	
	for (text_size = 0; data[start_index + text_size] != ',' || data[start_index + text_size] != '\0';text_size++);
	substr(data, temp, start_index, text_size);
	long plant_2_time = atoi(temp);
	start_index += text_size + 1;
	
	set_last_use_plants_time(plant_0_time, plant_1_time, plant_2_time);
	
	//zombie
	start_index += 2;
	
	
}



// give part
char buffer[100];
unsigned char buffer_size = 0;

void fill_buffer(char charecter){
	//printf("%c\n", charecter);
	
	if (charecter != '\n'){
		buffer[buffer_size] = charecter;
		buffer_size++;
		buffer[buffer_size] = '\0';
	}else{
		char temp[200];
		
		if (startWith(buffer, "pc:")){
			substr(buffer, temp, 3, 1);
			char kind = atoi(temp);
			substr(buffer, temp, 5,1);
			char row = atoi(temp);
			substr(buffer, temp, 7,2);
			char column = atoi(temp);
			if (get_state() == GAME || get_state() == NEW_GAME){
				create_plant(kind, row, column);
			}
			
		}else if(startWith(buffer, "state:")){
			substr(buffer, temp, 6,1);
			switch (atoi(temp)){
				case 0:
					set_state_without_uart(SHOW_MENU);
					break;
				case 1:
					set_state_without_uart(SHOW_ABOUT);
					break;
				case 2:
					set_state_without_uart(NEW_GAME);
					clear();
					HAL_Delay(CLEAR_DALAY);
					noBlink();
					enable_blink();
					break;
			}
		}else if(startWith(buffer, "l:")){
			substr(buffer, temp, 2,1);
			/// todo check if in load state
			load(temp);
		}
		
		
		buffer_size = 0;
		buffer[0] = '\0';
	}
}