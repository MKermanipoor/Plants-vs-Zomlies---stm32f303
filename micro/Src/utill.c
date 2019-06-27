double __rand = 0;
int __rand_count = 0;

int getRand(int range){
	return ((int)(__rand * range)) % range;
}

void setRandomCount(int randomCount){
	__rand_count = randomCount;
	__rand = (double)__rand_count / 59;
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