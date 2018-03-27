#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"
#include "Node.h"

Queue *newQueue() {
  Queue *q = malloc(sizeof(Queue));
  if (q == 0) {
		fprintf(stderr, "NOT ENOUGH MEMORY...\n");
		exit(-1);
	}
  q->front = NULL;
  q->back = NULL;
  return q;
}

void enqueue(Queue *q, Process *p) {
  Node *temp = newNode(p);
  if (q->front == NULL && q->back == NULL) {
    q->front = q->back = temp;
  } else {
    q->back->next = temp;
    q->back = temp;
  }
}

void sortedEnqueue(Queue *q, Process *p) {
  Node *temp = newNode(p);
  if (q->front == NULL && q->back == NULL) {
    q->front = q->back = temp;
  }
	else {
    if (q->front->process->arrival > p->arrival) {
      temp->next = q->front;
      q->front = temp;
    }
		else {
      Node *iter = q->front;
      while (iter->next != NULL && iter->next->process->arrival <= p->arrival) {
        iter = iter->next;
      }
      if (iter == q->back) {
        q->back->next = temp;
        q->back = temp;
      }
			else {
        temp->next = iter->next;
        iter->next = temp;
      }
    }
  }
}

Node *dequeue(Queue *q) {
  Node *temp = q->front;
  if (q->front == NULL) {
    printf("Queue is empty\n");
    return NULL;
  }
  if (q->front == q->back)
    q->front = q->back = NULL;
  else
    q->front = q->front->next;
  return temp;
}

int nonEmpty(Queue *q) {
  if (q->front != NULL)
    return 0;
  else
    return -1;
}
