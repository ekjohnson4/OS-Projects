#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t pid;

void collatz(int i);

int main(int argc, char** argv) {

    if ( argc == 2){
      // fork child process
  		pid = fork();

      // If zero or less
      if(atoi(argv[1]) <= 0){
        printf("Please enter a single positive integer greater than zero.\n");
        exit(0);
      }
  		// Error
  		if (pid < 0) {
  			fprintf(stderr, "Failed to Fork Process");
  			return 1;
  		}
  		// Child process
  		else if (pid == 0) {
  			collatz(atoi(argv[1]));
  		}
  		// Parent process
  		else {
  			wait(NULL);
              }
    }

    else{
      // Invalid input
      printf("Please enter a single positive integer.\n");
      return 0;
    }

    return 0;
}

void collatz(int i){
	if(i == 1){
		printf("%d\n",i);
	}
	// Even
	else if ( i % 2 == 0) {
		printf("%d\n",i);
		collatz( i / 2 );
	}
	// Odd
	else{
		printf("%d\n",i);
		collatz( 3 * i + 1 );
	}
}
