#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef enum{false, true}boolean;

typedef struct queuenode
{
  char info;
  char hash_char;
  struct queuenode *next;
  struct queuenode *prev;
}queuenode;

typedef struct queue
{
  int capacity_of_frames;
  int no_of_frames;
  queuenode *front;
  queuenode *rear;
}queue;

typedef struct map{
  queuenode *node;
}map;

map character_hash[256];

void create_queue(queue *q)
{
  q->no_of_frames = 0;
  printf("Please enter the capacity of cache: ");
  scanf("%d",&(q->capacity_of_frames));
  q->front = q->rear = NULL;
}

boolean is_queue_empty(queue *q)
{
  if(q->front == NULL)
    return true;
  return false;
}

void enqueue(queue *q, char hash_char, char value)
{
  queuenode *node = NULL;
  node=malloc(sizeof(queuenode));
  node->info = value;
  node->hash_char = hash_char;
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

  character_hash[hash_char].node = node;
}

void dequeue(queue *q)
{
  queuenode *node = NULL;
  int info = -1;
  if(is_queue_empty(q))
    return;
  node = q->front;
  if(node->next)
  {
    q->front = node->next;
    node->next->prev = NULL;
  }
  info = node->info;
  if(node == q->rear)
    q->rear = q->front = NULL;
  character_hash[node->hash_char].node = NULL;
  free(node);
}

void add_in_cache(queue *q, char hc, char x)
{

  if(q->no_of_frames < q->capacity_of_frames)
  {
    enqueue(q, hc, x);
    q->no_of_frames++;
  }
  else
  {
    dequeue(q); 
    enqueue(q, hc, x); 
  }
}

void get_char_and_manage_cache(queue *q, char *x)
{
  switch(*x)
  {
    case '!' : add_in_cache(q, '!', '1'); *x = '1';
    break;
    case '@' : add_in_cache(q, '@', '2'); *x = '2';
    break;
    case '#' : add_in_cache(q, '#', '3'); *x = '3';
    break;
    case '$' : add_in_cache(q, '$', '4'); *x = '4';
    break;
    case '%' : add_in_cache(q, '%', '5'); *x = '5';
    break;
    case '^' : add_in_cache(q, '^', '6'); *x = '6';
    break;
    case '&' : add_in_cache(q, '&', '7'); *x = '7';
    break;
    case '*' : add_in_cache(q, '*', '8'); *x = '8';
    break;
    case '(' : add_in_cache(q, '(', '9'); *x = '9';
    break;
    case ')' : add_in_cache(q, ')', '0'); *x = '0';
    break;
  }
}

boolean is_in_cache(char x)
{
  if(character_hash[x].node != NULL)
    return true;
  return false;
}

void get_from_cache_and_update_cache(queue *q, char *x)
{
  queuenode *node = character_hash[*x].node;
  *x = node->info;
  
  if(node->prev != NULL)
    node->prev->next = node->next;
  if(node->next != NULL)
    node->next->prev = node->prev;

  node->next = q->front;
  q->front->prev = node;
  q->front = node;
  node->prev = NULL;
}

int main(int argc, char * argv[])
{
  queue q;
  create_queue(&q);
  char *input = NULL;
  int i;
  
  if(argc > 1)
  {
    input = (char*)malloc(strlen(argv[1]));
    strcpy(input, argv[1]);
  }
  else
  {
    input = (char*)malloc(20);
    memset(input, 0, 20);
    strcpy(input, "Default!@#$%^&*(");
  }

  for(i=0; i<strlen(input); i++)
  {

    //Check if character is in cache?//True get from cache and update cache://False get from main memory and update in cache
    (is_in_cache(input[i]))?get_from_cache_and_update_cache(&q, &(input[i])):get_char_and_manage_cache(&q, &(input[i]));

  }

  printf("Output: %s\n",input);
}


/*int main()
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
}*/
