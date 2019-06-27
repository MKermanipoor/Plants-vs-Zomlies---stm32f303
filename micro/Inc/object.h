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