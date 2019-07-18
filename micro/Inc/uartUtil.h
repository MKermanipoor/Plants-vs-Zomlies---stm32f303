//zombie
void U_create_zombie(struct Zombie);
void U_remove_zombie(struct Zombie);
void U_move_zombie(struct Zombie);

//plant
void U_create_plant(struct Plant);
void U_remove_plant(struct Plant);
void U_enable_plant(char, long);

//bouns
void U_bouns_create(struct Bonus);
void U_bouns_remove(struct Bonus);

//ligth
void U_ligth(char);

//score
void U_score(int);

//round
void U_round(char);

//time
void U_time(long);
void U_game_time(long);

//life
void U_set_life(char);

//save
void U_save_game(long , long , char);

//state
void U_send_state(char);
void U_load_game();


void fill_buffer(char);