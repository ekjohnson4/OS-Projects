#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define HISTORY_SIZE 10
#define MIN(a,b)(a < b ? a : b)

int numOfCommands = 0;
char * history[HISTORY_SIZE];

void addCommand(char * buffer) {
  int i;
  char * entry = malloc(sizeof(char) * MAX_LINE);

  strcpy(entry, buffer);

  //Less than 10 commands in history
  if(numOfCommands < HISTORY_SIZE) {
    history[numOfCommands] = entry;
  }
  //Get last 10 commands from history
  else {
    //Remove oldest history entry
    free(history[0]);
    for(i = 1; i < HISTORY_SIZE; i++) {
      //Shift all entries over one space
      history[i-1] = history[i];
    }
    //Set new command as most recent history entry
    history[HISTORY_SIZE - 1] = entry;
  }
  numOfCommands++;
}

//Gets number of arguments
int process(char * buffer, char * args[]) {
  //If buffer does not equal 'history' or include '!'
  if(strncmp("history",buffer,7) && (buffer[0] != '!')) {
    addCommand(buffer);
  }

  //Add buffer tokens to array
  int count = 0;
  args[0] = strtok(buffer, " \n");
  while(args[count] != NULL) {
    count++;
    args[count] = strtok(NULL, " \n");
  }
  return count;
}

void runCommand(char * args[], int length) {
  pid_t pid;
  int pause = 0;

  if(!strcmp(args[length-1], "&")) {
    pause = 1;
    args[length-1] = NULL;
  }
  else {
    pause = 0;
  }

  //(1) fork a child process using fork()
  pid = fork();
  if(!pid) {
    //(2) the child process will invoke execvp()
    execvp(args[0], args);
    exit(0);
  }
  else {
    //(3) if command included &, parent will NOT invoke wait()
    if(pause) {
      waitpid(pid, NULL, 0);
    }
  }
}

void getCommands(int command) {
  char* args[MAX_LINE/2 + 1];
  char buffer[MAX_LINE];
  int length;

  //If command number exists within last 10 history commands
  if(command <= numOfCommands && (numOfCommands - command) < 10) {
    if(numOfCommands < 10) {
      strcpy(buffer, history[command-1]);
    }
    else {
      strcpy(buffer, history[(10 - (numOfCommands - command)) -1]);
    }
    length = process(buffer, args);
    runCommand(args, length);
  }
  //Command number does not exist
  else {
    puts("No such command in history.");
  }
}

void printHistory() {
  int c = numOfCommands, i;

  if(numOfCommands == 0) {
    printf("No commands in history.\n");
  }
  //Prints either 10 (HISTORY_SIZE) or less than 10(numOfCommands)
  for(i = (MIN(HISTORY_SIZE,numOfCommands))-1; i >= 0; i--) {
    printf("%d %s", c, history[i]);
    c--;
  }
}

int main(void) {
  // command line args
  char* args[MAX_LINE/2 + 1];
  // flag for exiting program
  int should_run = 1;
  // command length
  int length = 0;
  //buffer with length of 80 to store user command
  char buffer[MAX_LINE];

  while(should_run) {
    printf("osh> ");
    fflush(stdout);

    if(fgets(buffer, MAX_LINE-1, stdin)) {
      length = process(buffer, args);

      if(args[0] != NULL) {
        //Exits program
        if(!strcmp("exit", args[0])) {
          should_run = 0;
        }
        //Displays command history
        else if(!strcmp("history", args[0])) {
          printHistory();
        }
        //Runs a previous command
        else if(args[0][0] == '!') {
          if(args[0][1] == '!') {
            //Run last command
            getCommands(numOfCommands);
          }
          else {
            //Run command based on provided argument
            getCommands(atoi(&args[0][1]));
          }
        }
        //Runs a new command
        else {
          runCommand(args, length);
        }
      }
    }
  }

  return 0;
}
