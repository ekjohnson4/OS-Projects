#include <stdio.h>
#include <string.h>
#include "pid_manager.h"

int allocate_map(){
    for(int i = 0; i < MAX_PID; i++){
            pid_map[i] = 0;
    }

    id = pthread_mutex_init(&mutex, NULL);
    if(id!=0){
      id = -1;
    }
    return id;
}

int allocate_pid(){
    while(id!=0){
      id = pthread_mutex_lock(&mutex);
    }

    if(available < 1){
      return -1;
    }

    for(int i = 0; i <= MAX_PID; i++){
            if(pid_map[i] == 0){
                available--;
                pid_map[i] = 1;
                id = pthread_mutex_unlock(&mutex);
                return i + MIN_PID;
            }
    }
    return id;
}

void release_pid(int pid){
    id = pthread_mutex_lock(&mutex);

    if(pid < MIN_PID || pid > MAX_PID){
      printf("Error. Invalid PID\n");
      return;
    }

    if(id!=0){
      return;
    }
    pid_map[MIN_PID + pid] = 0;
    available++;
    id = pthread_mutex_unlock(&mutex);
}
