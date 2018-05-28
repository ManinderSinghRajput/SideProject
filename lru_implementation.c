#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct queuenode
{
  int info;
  struct queuenode *next;
  struct queuenode *prev;
}queuenode;


typedef struct queue
{
  queuenode *front;
  queuenode *rear;
}queue;

void create_queue(queue *q)
{
  q->front = q->rear = NULL;
}

int is_queue_empty(queue *q)
{
  if(q->front == NULL)
    return 1;
  return 0;
}

void enqueue(queue *q, int value)
{
  queuenode *node = NULL;
  node=malloc(sizeof(queuenode));
  node->info = value;
  node->next = NULL;
  node->prev = NULL;
  if(is_queue_empty(q))
  {
    q->front = node;
    q->rear = node;
  }
  else
  {
    node->next = q->front;
    q->front->prev = node;
    q->front = node;
  }
}

int dequeue(queue *q)
{
  queuenode *node = NULL;
  int info = -1;
  if(is_queue_empty(q))
    return -1;
  node = q->front;
  if(node->next)
  {
    q->front = node->next;
    node->next->prev = NULL;
  }
  info = node->info;
  if(node == q->rear)
    q->rear = q->front = NULL;
  free(node);
  return info;
}

int main()
{
  queue q;

  create_queue(&q);

  enqueue(&q,1);
  enqueue(&q,8);
  enqueue(&q,0);
  enqueue(&q,5);

  printf("%d\n", dequeue(&q));
  printf("%d\n", dequeue(&q));
  printf("%d\n", dequeue(&q));
  printf("%d\n", dequeue(&q));
  printf("%d\n", dequeue(&q));
}
