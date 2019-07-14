#include "time.h"

char __name[19];
char __name_index = 0;

char __last_key_row_press = -1;
char __last_key_column_press = -1;
long __last_key_press_time = 0;
char __char_select_index = 0;

const char charArr[3][3][4] = {
	{
		{' ','1','1','1'},
		{'A','B','C','1'},
		{'D','E','F','1'},
	},
	{
		{'G','H','I','1'},
		{'J','K','L','1'},
		{'M','N','O','1'},
	},
	{
		{'P','Q','R','S'},
		{'T','U','V','1'},
		{'W','X','Y','Z'},
	},
};

const char charArrsize [3][3] = {
  {1,3,3},
	{3,3,3},
	{4,3,4},
};

void reset_keypad(){
	__name_index = 0;
	__last_key_column_press = -1;
	__last_key_row_press = -1;
	__last_key_press_time = 0;
}
void get_name(char name[]){
	for (char i=0;i<__name_index;i++)
		name[i] = __name[i];
	
	name[__name_index] = '\0';
}

void press_keypad(char row, char column){
	if (row > 2 || column > 2)
		return;
	
	if (__last_key_column_press != column || __last_key_row_press != row || getTime() - __last_key_press_time > 1.5 * TIME_TO_SEC){
		__char_select_index = 0;
	}else{
		__char_select_index = (__char_select_index + 1) % charArrsize[row][column];
		__name_index --;
	}
	
	__last_key_column_press = column;
	__last_key_row_press = row;
	__last_key_press_time = getTime();
	
	
	__name[__name_index] = charArr[row][column][__char_select_index];
	__name_index ++;
}