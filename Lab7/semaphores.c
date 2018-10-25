//maximim value of a counting semaphore


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <limits.h>
#include <sys/param.h>

int main(){
	//create semaphore
	//loop til it returns -1
	
	sem_t x;
	sem_init(&x,0,1);
	int temp;
	while(sem_post(&x)>=0);
	sem_getvalue(&x, &temp);
	printf("SEM LIMIT %i \n",temp);
	sem_close(&x);		

}
