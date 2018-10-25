#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/shm.h>
#include <sys/stat.h>

int main(){

	int shmId;
	long size = 21058577350;
	while(1){
		if((shmId = shmget(IPC_PRIVATE, size, IPC_CREAT)) < 0){
			printf("MAX SIZE: %lu\n ",size);
			exit(0);
		}
		shmctl(shmId,IPC_RMID, 0);
		size = size + 1;
	}


}
