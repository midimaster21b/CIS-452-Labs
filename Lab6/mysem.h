
#ifndef __MYSEM_H__
#define __MYSEM_H__

// create
int  sem_create(void);

// initialize
void sem_init(int sem_id);

// wait
void sem_wait(int sem_id);

// signal
void sem_signal(int sem_id);

// destroy
void sem_destroy(int sem_id);

#endif /* __MYSEM_H__ */
