#ifndef PID_MANAGER_H
#define PID_MANAGER_H
#include <pthread.h>

int MAX_PID = 5000;
int MIN_PID = 300;
int RANGE = 4700;
int pid_map[4700];
int available = 4700;
int x = 5;
int id = 0;

int allocate_pid(void);
int allocate_map(void);
void release_pid(int pid);

#endif
