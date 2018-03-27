#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "Queue.h"
#include "Process.h"

int t = 0;

void terminateProcess(Process *p) {
  int status;
  kill(p->pid, SIGINT);
  waitpid(p->pid, &status, WUNTRACED);
  }

void suspendProcess(Process *p) {
  int status;
  kill(p->pid, SIGTSTP);
  waitpid(p->pid, &status, WUNTRACED);
  }

void restartProcess(Process *p) {
  kill(p->pid, SIGCONT);
  }

void startProcess(Process *p) {
  if (p->pid == 0) {
    p->pid = fork();
    if (p->pid < 0) {
      fprintf(stderr, "Fork has failed!\n");
      exit(EXIT_FAILURE);
    }
    else if (p->pid == 0) {
      char *args[3] = {NULL};
      char ptime[50];
      sprintf(ptime, "%d", p->ptime);
      args[0] = "./process";
      args[1] = ptime;
      int err = execvp(args[0], args);
      if (err == -1) {
        puts("Process command failed. Now exiting.");
        exit(EXIT_FAILURE);
      }
    }
  }
  else {
    restartProcess(p);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("Need argument with input file!");
    exit(EXIT_FAILURE);
  }

  Queue *d = newQueue(), *s = newQueue(), *qa = newQueue(), *qb = newQueue(), *qc = newQueue();
  FILE *fp;
  char *lin = NULL;
  size_t l = 0;
  ssize_t read;

  fp = fopen(argv[1], "r");
  if (fp == NULL){
    exit(EXIT_FAILURE);
  }

  //Get numbers from input file
  while ((read = getline(&lin, &l, fp)) != -1) {
    char *token;
    token = strtok(lin, ",");
    int arrival = atoi(token);
    token = strtok(NULL, ",");
    int priority = atoi(token);
    token = strtok(NULL, ",");
    int ptime = atoi(token);
    token = strtok(NULL, ",");
    Process *p = newProcess(arrival, priority, ptime);
    sortedEnqueue(d, p);
    token = strtok(NULL, ",");
  }

  fclose(fp);
  if (lin){
    free(lin);
  }

  Process *currentPro = NULL;
  Node *node = NULL;
  while (nonEmpty(d) == 0 || nonEmpty(s) == 0 || nonEmpty(qa) == 0 || nonEmpty(qb) == 0 || nonEmpty(qc) == 0 || currentPro != NULL) {
    while (d->front != NULL && d->front->process->arrival <= t) {
      node = dequeue(d);
      switch (node->process->priority) {
        case 0: enqueue(s, node->process); break;
        case 1: enqueue(qa, node->process); break;
        case 2: enqueue(qb, node->process); break;
        case 3: enqueue(qc, node->process); break;
      }
      node = NULL;
    }
    if (currentPro != NULL) {
      currentPro->ptime--;
      if (currentPro->ptime <= 0) {
        terminateProcess(currentPro);
        currentPro = NULL;
      }
      else if (nonEmpty(s) == 0 || nonEmpty(qa) == 0 || nonEmpty(qb) == 0 || nonEmpty(qc) == 0) {
        suspendProcess(currentPro);
        if (currentPro->priority <= 3) {
          if (currentPro->priority < 3) {
            currentPro->priority++;
          }
          switch (currentPro->priority) {
            case 2: enqueue(qb, currentPro); break;
            case 3: enqueue(qc, currentPro); break;
          }
        }
        currentPro = NULL;
      }
      else {
        suspendProcess(currentPro);
        startProcess(currentPro);
        sleep(1);
      }
    }
    else if (currentPro == NULL) {
      Node *temp = NULL;
      if (nonEmpty(s) == 0) {
        temp = dequeue(s);
        currentPro = temp->process;
        startProcess(currentPro);
        sleep(currentPro->ptime);
        t = t + currentPro->ptime;
        terminateProcess(currentPro);
        currentPro = NULL;
      }
      else if (nonEmpty(qa) == 0) {
        temp = dequeue(qa);
        currentPro = temp->process;
        startProcess(currentPro);
        sleep(1);
        t++;
      }
      else if (nonEmpty(qb) == 0) {
        temp = dequeue(qb);
        currentPro = temp->process;
        startProcess(currentPro);
        sleep(1);
        t++;
      }
      else if (nonEmpty(qc) == 0) {
        temp = dequeue(qc);
        currentPro = temp->process;
        startProcess(currentPro);
        sleep(1);
        t++;
      }
      else {
        t++;
      }
    }
  }
  exit(EXIT_SUCCESS);
}
