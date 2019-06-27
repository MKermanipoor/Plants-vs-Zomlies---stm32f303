struct Plant{
	unsigned char row;
	unsigned char column;
	unsigned char kind;
	unsigned char hp;
	unsigned char id;
};

struct Plant create_plant(char , char , char );
char get_plant_size();
struct Plant * get_plant(char);
void remove_plant(struct Plant plant);
void print_all_plant();

struct Zombie{
	long lastTimeMove;
	unsigned char row;
	unsigned char column;
	unsigned char before_row;
	unsigned char before_column;
	unsigned char kind;
	unsigned char hp;
	unsigned char id;
};

struct Zombie create_zombie(char);
void move_all_zombies();
void print_all_zombies();