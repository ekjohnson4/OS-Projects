#include <stdio.h>
#include <string.h>
#include "pid_manager.h"

int allocate_map(){
    for(int i = 0; i < MAX_PID; i++){
            pid_map[i] = 0;
    }
    return id;
}

int allocate_pid(){
    if(available < 1){
      return -1;
    }

    for(int i = 0; i <= MAX_PID; i++){
            if(pid_map[i] == 0){
                available--;
                pid_map[i] = 1;
                return i + MIN_PID;
            }
    }
    return id;
}

void release_pid(int pid){
    if(pid < MIN_PID || pid > MAX_PID){
      printf("Error. Invalid PID\n");
      return;
    }

    pid_map[MIN_PID + pid] = 0;
    available++;
}
