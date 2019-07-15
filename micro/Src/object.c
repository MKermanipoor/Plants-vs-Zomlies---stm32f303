#include "object.h"
#include "LCDutill.h"
#include "utill.h"
#include "time.h"
#include "player.h"
#include "uartUtil.h"

// plant struct
unsigned char __plant_size = 0;
struct Plant __plants [20];

char __start_timer = 0;

long __last_create_plant[3];

const long PLANT1_TIME_LIMIT = TIME_TO_SEC * 4;
const long PLANT2_TIME_LIMIT = TIME_TO_SEC * 8;
const long PLANT3_TIME_LIMIT = TIME_TO_SEC * 10;

char get_plant_enable(char);

struct Plant create_plant(char kind, char row, char column){
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
	
	if(__plant_size < 20){
		for (char i=0 ; i<__plant_size ; i++){
			if (__plants[i].column == column && __plants[i].row == row){
				return result;
			}
		}
		
		if (get_plant_enable(result.kind)){
			result.id = __plant_size;
			__plants[__plant_size] = result;
			__plant_size++;
			U_create_plant(result);
			if (__start_timer){
				__last_create_plant[result.kind-1] = getTime();
			}
		}
	}
	
	
	return result;
}

struct Plant create_plant_with_hp(char kind, char row, char column, char hp){
	struct Plant result;
	result.column = column;
	result.row = row;
	result.kind = kind;
	result.hp = hp;
	
	if(__plant_size < 20){
		for (char i=0 ; i<__plant_size ; i++){
			if (__plants[i].column == column && __plants[i].row == row){
				return result;
			}
		}
		
		if (get_plant_enable(result.kind)){
			result.id = __plant_size;
			__plants[__plant_size] = result;
			__plant_size++;
			U_create_plant(result);
			if (__start_timer){
				__last_create_plant[result.kind-1] = getTime();
			}
		}
	}
	
	return result;
}

void remove_plant(struct Plant plant){
	if (plant.id < 0)
		return;
	
	if (plant.id > __plant_size)
		return;
	
	__plant_size --;
	U_remove_plant(__plants[plant.id]);
	remove_wink(__plants[plant.id].row, __plants[plant.id].column);
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

struct Plant * get_plant(char index){
	return &__plants[index];
}

char get_plant_size(){
	return __plant_size;
}

void start_plant_timer(){
	__start_timer = 1;
}
char get_plant_enable(char p){
	if (__start_timer){
		switch(p){
			case 1:
				return getTime() - __last_create_plant[0] > PLANT1_TIME_LIMIT;
			case 2:
				return getTime() - __last_create_plant[1] > PLANT2_TIME_LIMIT;
			case 3:
				return getTime() - __last_create_plant[2] > PLANT3_TIME_LIMIT;
		}
	}else
		return 1;
}

long get_last_use_plant_time(char index){
	return __last_create_plant[index];
}

void set_last_use_plants_time(long p_0, long p_1, long p_2){
	__last_create_plant[0] = p_0;
	__last_create_plant[1] = p_1;
	__last_create_plant[2] = p_2;
}
//zombies data
struct Zombie __zombies [10];
unsigned char __zombies_size = 0;

unsigned int speed_time = 1.5 * TIME_TO_SEC;

struct Zombie create_zombie(char kind){
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
	
	result.lastTimeMove = getTime();
	
	for(int i=0; i<__zombies_size; i++){
		if(result.column == __zombies[i].column && result.row == __zombies[i].row)
			return result;
	}
	
	if(__zombies_size < 10){
		result.id = __zombies_size;
		__zombies[__zombies_size] = result;
		__zombies_size++;
		U_create_zombie(result);
	}
	return result;
}

void removeZombie(struct Zombie z){
	if (z.id < 0)
		return;
	
	if (z.id > __zombies_size)
		return;
	
	__zombies_size --;
	remove_wink(__zombies[z.id].row-1, __zombies[z.id].column);
	U_remove_zombie(__zombies[z.id]);
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
	if(getTime() - z->lastTimeMove > speed_time){
		z->row++;
		z->lastTimeMove = getTime();
		
		for (char i=0; i<get_plant_size(); i++){
			if(get_plant(i)->column == z->column && get_plant(i)->row == z->row){
				char temp = min(get_plant(i)->hp, z->hp);
				
				get_plant(i)->hp -= temp;
				if(get_plant(i)->hp <= 0){
					remove_plant(*get_plant(i));
					remove_wink(get_plant(i)->row-1, get_plant(i)->column);
				}
				
				z->hp -= temp;
				if(z->hp <= 0){
					removeZombie(*z);
					remove_wink(z->row, z->column);
				}
				break;
			}else if(get_plant(i)->column == z->column && get_plant(i)->row == z->row+1){
				add_wink(z->row, z->column);
				add_wink(z->row+1, z->column);
				break;
			}
		}
		
		if(z->row >= 4){
			removeZombie(*z);
			lost_hp();
		}
		
		if (z->row <4 && z->hp > 0){
			U_move_zombie(*z);
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

char get_zombie_size(){
	return __zombies_size;
}

struct Zombie get_zombie(char index){
	if (index >= __zombies_size)
		return __zombies[0];
	
	return __zombies[index];
}

//bouns
const long BONUS_TIME = 4 * TIME_TO_SEC;
struct Bonus __bonus;
long __bonus_created_time = 0;

void create_bonus(){
	if (!__bonus.kind){
		
		__bonus.row = getRand(4);
		__bonus.column = getRand(20);
//		__bonus.row = 0;
//		__bonus.column = 0;
		for (char i=0; i<__zombies_size;i++){
			if (__bonus.row == get_blink_row() && __bonus.column == get_blink_column()){
				return;
			}
		}
		__bonus_created_time = getTime();
		__bonus.kind = getRand(3)+1;
		__bonus.kind = 3;
		U_bouns_create(__bonus);
	}
}

void get_bonus_point(){
	char max_zombie_id = -1;
	switch(__bonus.kind){
		case 1:
			for (char i=0; i<3; i++){
				if(!get_plant_enable(i)){
					__last_create_plant[i] = 0;
				}
			}
			break;
			
		case 2:
			add_score(100);
			break;
		
		case 3:
			
			for (char i=0; i<__zombies_size; i++){
				if(max_zombie_id == -1){
					max_zombie_id = i;
					continue;
				}
				
				if(__zombies[max_zombie_id].row < __zombies[i].row){
					max_zombie_id = i;
					continue;
				}
				
				if(__zombies[max_zombie_id].row == __zombies[i].row &&
					__zombies[max_zombie_id].hp < __zombies[i].hp){
					max_zombie_id = i;
					continue;
				}
			}
			
			if (max_zombie_id > -1){
				removeZombie(__zombies[max_zombie_id]);
			}
			break;
	}
}

void remove_bonus(){
	__bonus.kind = 0;
		write_lcd(32, __bonus.column, __bonus.row);
	U_bouns_remove(__bonus);
}

void check_bonus(){
	if (__bonus.kind && __bonus.column == get_blink_column() && __bonus.row == get_blink_row()){
		get_bonus_point();
		remove_bonus();
	}
}

void print_bonus(){
	if (getTime() - __bonus_created_time > BONUS_TIME){
		remove_bonus();
	}
	
	if(__bonus.kind){
		write_lcd(7, __bonus.column, __bonus.row);
	}
}