double __rand[40];
char __end_rand = 40;
double __rand_now;

int getRand(int range){
	int result = ((int)(__rand[__end_rand] * range)) % range;
	__end_rand = (__end_rand + 39) % 40;
	return result;
}

void setRandomCount(int randomCount){
	double t = (double)randomCount / 128;
	if (__rand[__end_rand] != t){
		__end_rand = (__end_rand + 1) % 40;
		__rand[__end_rand] = t;
		__rand_now = __rand[__end_rand];
	}
}

char min(char a, char b){
	if (a < b)
		return a;
	return b;
}

unsigned long __time_m =0 ;
long getTime(){
	return __time_m;
}
void addTime(){
	__time_m ++;
}

char get_boolean(char p){
	return getRand(1000) < p;
}