#include "object.h"
#include "keypadUtil.h"
#include "player.h"
#include "utill.h"
#include "stm32f3xx_hal.h"
#include "state.h"
#include "time.h"
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
	printf("s\n{");
	
	
	char name[20];
	get_name(name);
	printf("\"n\":%s,", name); 
	
	//plant
	
	printf("\"p\":[");
	for (char i=0; i<get_plant_size(); i++){
		struct Plant p = *get_plant(i);
		printf("{\"c\":%d, \"r\":%d, \"hp\":%d}", p.column, p.row, p.hp);
		if (i!= get_plant_size()-1 ){
			printf(",");
		}
	}
	printf("],");
	
	//plant time
	printf("\"p_t\":[%ld,%ld,%ld],", get_last_use_plant_time(0), get_last_use_plant_time(1), get_last_use_plant_time(2));
	
	//zombie
	printf("\"z\":[");
	for (char i=0 ; i<get_zombie_size() ; i++){
		struct Zombie z = get_zombie(i);
		printf("{\"c\":%d, \"r\":%d, \"hp\":%d}", z.column, z.row, z.hp);
		if (i!= get_zombie_size()-1 ){
			printf(",");
		}
	}
	printf("],");
	
	// remaning hp
	printf("\"health\":%d, ", get_hp());
	
	// start geme time and level start time
	printf("\"g_t\":%ld, \"l_t\":%ld,", game_time, level_time);
	
	// level
	printf("\"l\":%d,", level);
	
	// time
	printf("\"t\":%ld,", getTime());
	
	//score
	
	printf("\"score\":%d", get_score());
	
	printf("}\n");
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
		char temp[10];
		
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
			
		}
		
		
		buffer_size = 0;
		buffer[0] = '\0';
	}
}
	



