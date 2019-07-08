#define CLEAR_DALAY 10

void write_lcd(char , char , char );

void show_blink();

char get_blink_row();
char get_blink_column();
void set_blink_row(char);
void set_blink_column(char);
void add_blink_row(char);
void add_blink_column(char);

char is_blink_enable();
void enable_blink();
void disable_blink();

void clear_lcd();