#include "object.h"
#include "keypadUtil.h"
#include "player.h"
#include "utill.h"
#include "stm32f3xx_hal.h"

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
	printf("zc: %d,%d,%d\n", z.column, z.row, z.kind);
}

void U_remove_zombie(struct Zombie z){
	printf("zr: %d,%d,%d\n", z.column, z.row, z.kind);
}

void U_move_zombie(struct Zombie z){
	printf("zm: %d,%d,%d,%d,%d\n", z.column, z.row, z.before_column, z.before_row, z.kind);
}

//plant
void U_create_plant(struct Plant p){
	printf("pc: %d,%d,%d\n", p.row, p.column, p.kind);
}
void U_remove_plant(struct Plant p){
	printf("pr: %d,%d,%d\n", p.row, p.column, p.kind);
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