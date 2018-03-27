#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

//Global variables
int avg_int=0;
int min_int=0;
int max_int=0;
int size=0;

void *getAvg(void *bin){
  int *in = (int*)bin;
  int total = 0;

  for (int it = 0; it < size; ++it){
      total += in[it];
    }

  avg_int = total / size;
  pthread_exit(0);
}


void *getMin(void *bin){
  int *in = (int*)bin;
  min_int = in[0];
  for (int it = 0; it < size; ++it){
      if (in[it] < min_int)
      min_int = in[it];
    }
  pthread_exit(0);
}

void *getMax(void *bin){
  int *in = (int*)bin;
  max_int = in[0];
  for (int it = 0; it < size; ++it){
      if (in[it] > max_int)
      max_int = in[it];
    }
  pthread_exit(0);
  }

int main(int argc, char** argv) {
    pthread_attr_t attr1,attr2,attr3;
    pthread_t id1,id2,id3;
    size = argc - 1;
    int* data  = (int*)calloc( size, sizeof(int) ) ;

    //Arg check
    if(argc<1 || !argv){
      printf("Provide integers.");
      return -1;
    }

    //Take in arguments and store in array
    for(int i = 0; i < size; i++){
        sscanf( argv[i + 1], "%d", &data[i] ) ;
    }

    //Initialize attributes
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);

    //Create 3 threads for avg, min, and max
    pthread_create(&id1,&attr1,getAvg,(void*)data);
    pthread_create(&id2,&attr2,getMin,(void*)data);
    pthread_create(&id3,&attr3,getMax,(void*)data);

    //Terminate threads
    pthread_join(id1,NULL);
    pthread_join(id2,NULL);
    pthread_join(id3,NULL);

    //Print results
    printf("The Average value is %d\n",avg_int);
    printf("The Minimum value is %d\n",min_int);
    printf("The Maximum value is %d\n",max_int);

    return 0;
}
