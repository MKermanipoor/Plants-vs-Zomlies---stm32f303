struct Plant{
	unsigned char row;
	unsigned char column;
	unsigned char kind;
	unsigned char hp;
	unsigned char id;
};

struct Plant create_plant(char , char , char);
struct Plant create_plant_with_hp(char , char , char, char);
char get_plant_size();
struct Plant * get_plant(char);
void remove_plant(struct Plant plant);
void print_all_plant();
void start_plant_timer();
char get_plant_enable(char);
long get_last_use_plant_time(char);
void set_last_use_plants_time(long, long, long);

struct Zombie{
	long lastTimeMove;
	unsigned char row;
	unsigned char column;
	unsigned char before_row;
	unsigned char before_column;
	unsigned char kind;
	unsigned char hp;
	char id;
};

struct Zombie create_zombie(char);
struct Zombie create_zombie_custome(char , char, char, char, long);
void move_all_zombies();
void print_all_zombies();
char get_zombie_size();
struct Zombie get_zombie(char);
void set_zombie_move_time_by_level(char);

struct Bonus{
	unsigned char row;
	unsigned char column;
	unsigned char kind;
};

void create_bonus();
void check_bonus();
void print_bonus();
void click_bonus();