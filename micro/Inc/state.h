#define SHOW_DEMO 0
#define SHOW_MENU 1
#define SHOW_ABOUT 2
#define NEW_GAME 3
#define TEMP -1
#define GAME 4
#define GAME_OVER 5
#define SAVE_GAME 6
#define WIN 7


void set_state(char);
void set_state_without_uart(char);
char get_state();