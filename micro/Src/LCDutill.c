#include "stm32f3xx_hal.h"
#include "LiquidCrystal.h"
#include "utill.h"


unsigned char lcd [20][4];
unsigned char __input_row_blink = 0;
unsigned char __input_column_blink = 0;
unsigned char row_blink = 0;
unsigned char column_blink = 0;
unsigned char last_row_blink = 0;
unsigned char last_column_blink = 0;
char blinkEnable = 0;
char blinkChange = 0;
unsigned char __now_blink_enable = 0;
long __last_change_blink = 0;
const long timeToBlink = TIME_TO_SEC/ 6;

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
		if(getTime() - __last_change_blink > timeToBlink){
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
			__last_change_blink = getTime();
		}
	}else{
		__now_blink_enable = 0;
	}
}

char get_blink_row(){
	return row_blink;
}

char get_blink_column(){
	return column_blink;
}

void set_blink_row(char row){
	if (blinkEnable)
		__input_row_blink = row;
	else
		row_blink = row;
}

void set_blink_column(char column){
	if (blinkEnable)
		__input_column_blink = column;
	else
		column_blink = column;
}

void add_blink_row(char add){
	if (blinkEnable){
		__input_row_blink += add;
		__input_row_blink = __input_row_blink % 4;
	}else{
		row_blink += add;
		row_blink = row_blink % 4;
	}
}

void add_blink_column(char add){
	if (blinkEnable){
		__input_column_blink += add;
		__input_column_blink = __input_column_blink % 4;
	}else{
		column_blink += add;
		column_blink = column_blink % 4;
	}
}

char is_blink_enable(){
	return blinkEnable;
}
	
void enable_blink(){
	blinkEnable = 1;
}
	
void disable_blink(){
	blinkEnable = 0;
}

void set_blink_change(char state){
	blinkChange = state;
}

char is_blick_change(){
	return blinkChange;
}

void clear_lcd(){
	for (char i=0 ; i<20; i++)
		for (char j=0; j<4; j++)
			lcd[i][j] = 32;
}