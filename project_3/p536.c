#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include "pid_manager.c"

void *test(void *args){
    //Acquire random time to sleep
    int sTime = *((unsigned int *) args);

    //Request pid
    int p = allocate_pid();

    //Sleep for random amount of time
    sleep(sTime);
    counter++;
    printf("Sleeping time: %d secs ; Thread Id = %d; Counter value = %d;\n",sTime,p,counter);

    //Release pid
    release_pid(p);

    return NULL;
}

int main(){
    int x = 30;
    int MAXTIME = 10;
    int sTime[x];
    pthread_t t[x];
    id = allocate_map();

    //Check for failure
    if(id == -1){
      printf("Failure.\n");
      return -1;
    }

    for(int i = 0; i < x; i++){
        sTime[i] = (rand() % MAXTIME);
    }

    for(int i = 0; i < x; i++){
        id = pthread_create(&t[i], NULL, test,(void *) &sTime[i]);
        if(id == -1){
          return -1;
        }
    }

    for(int i = 0; i < x; i++){
        id = pthread_join(t[i], NULL);
        if(id == -1){
          return -1;
        }
    }
    wait(NULL);
    exit(0);
}
