#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "pid_manager.c"

int main(){
    id = allocate_map();

    //Check for failure
    if(id == -1){
      printf("Failure. Could not initalize pidmap.\n");
      return -1;
    }
    else{
      printf("Success.\n");
    }

    //Show availability of identifiers
    for(int i = 1; i != x; i++){
        id = allocate_pid();
        printf("pid %d allocated\n",id);

        release_pid(id);
        printf("pid %d released\n",id);
    }
    printf("pids available: %d\n",available);

    return 0;
}
