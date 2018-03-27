#ifndef PID_MANAGER_H
#define PID_MANAGER_H
#include <pthread.h>

int MAX_PID = 5000;
int MIN_PID = 500;
int RANGE = 4500;
int pid_map[4500];
int available = 499;
int id = 1;
int counter = 0;
pthread_mutex_t mutex;

int allocate_pid(void);
int allocate_map(void);
void release_pid(int pid);

#endif
