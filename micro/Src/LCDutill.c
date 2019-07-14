#include "stm32f3xx_hal.h"
#include "LiquidCrystal.h"
#include "utill.h"
#include "time.h"

//blink data
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
const long timeToBlink = TIME_TO_SEC/2;

//wink data
unsigned char wink_cells[4][20];
unsigned long last_time_wink = 0;
unsigned char __wink_enable = 0;
unsigned char __now_wink = 0;
const long timeToWink = TIME_TO_SEC/5;


void write_lcd(char c, char column, char row){
	lcd[column][row] = c;
	if((!blinkEnable || column != column_blink || row != row_blink) && !__wink_enable){
		setCursor(column, row);
		write(c);
	}
}

void show_blink(){
	if(blinkChange && (last_column_blink != __input_column_blink || last_row_blink != __input_row_blink)){
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
	if (blinkEnable){
		__input_row_blink = row;
		blinkChange = 1;
	}else
		row_blink = row;
}

void set_blink_column(char column){
	if (blinkEnable){
		__input_column_blink = column;
		blinkChange = 1;
	}else
		column_blink = column;
}

void add_blink_row(char add){
	if (blinkEnable){
		__input_row_blink += add;
		__input_row_blink = __input_row_blink % 4;
		blinkChange = 1;
	}else{
		row_blink += add;
		row_blink = row_blink % 4;
	}
}

void add_blink_column(char add){
	if (blinkEnable){
		__input_column_blink += add;
		__input_column_blink = __input_column_blink % 4;
		blinkChange = 1;
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





void add_wink (char row, char column){
	if (row < 4 && column < 20){
		wink_cells[row][column] = 1;
	}
}

void remove_wink (char row, char column){
	if (row < 4 && column < 20){
		wink_cells[row][column] = 0;		
	}
}

void remove_all_wink(){
	for (char i=0 ; i<4; i++){
		for (char j=0 ; j<20 ; j++){
			wink_cells[i][j] = 0;
		}
	}

}

void refresh_wink(){
	if (!__wink_enable)
		return;
	
	if (getTime() - last_time_wink > timeToWink){
		__now_wink = !__now_wink;
		last_time_wink = getTime();
		for (char i=0 ; i<4; i++){
			for (char j=0; j<20; j++){
				setCursor(i,j);
				if (__now_wink){
					write(32);
				}else{
					write(lcd[i][j]);
				}
			}
		}
	}
}

void enable_wink(){
	__wink_enable = 0;
}

void disable_wink(){
	__wink_enable = 0;
	__now_wink = 0;
}


void clear_lcd(){
	for (char i=0 ; i<20; i++)
		for (char j=0; j<4; j++)
			lcd[i][j] = 32;
}