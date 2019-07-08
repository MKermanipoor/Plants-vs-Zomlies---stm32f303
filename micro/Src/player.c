
int __score = 0;

char __hp = 5;

int get_score(){
	return __score;
}
void add_score(char s){
	__score += s;
}
void reset_score(){
	__score = 0;
}

void reset_hp(){
	__hp = 5;
}
char get_hp(){
	return __hp;
}

void lost_hp(){
	if (__hp > 0 )
		__hp --;
}