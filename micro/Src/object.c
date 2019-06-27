#include "object.h"
#include "LCDutill.h"
#include "utill.h"

// plant struct
unsigned char __plant_size = 0;
struct Plant __plants [10];

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

struct Plant * get_plant(char index){
	return &__plants[index];
}

char get_plant_size(){
	return __plant_size;
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
	if(getTime() - z->lastTimeMove > speed_time){
		z->row++;
		z->lastTimeMove = getTime();
		
		for (char i=0; i<get_plant_size(); i++){
			if(get_plant(i)->column == z->column && get_plant(i)->row == z->row){
				char temp = min(get_plant(i)->hp, z->hp);
				get_plant(i)->hp -= temp;
				if(get_plant(i)->hp <= 0)
					remove_plant(*get_plant(i));
				
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