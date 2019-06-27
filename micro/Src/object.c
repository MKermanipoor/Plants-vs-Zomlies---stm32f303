#include "object.h"
#include "LCDutill.h"

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