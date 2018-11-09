#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct listnode
{
  struct listnode *next;
  double value;
} Node;

/* Creates a node */
static Node* Node_construct(double val)
{
  Node *nd = malloc(sizeof(Node));
  nd->value = val;
  nd->next = NULL;
  return nd;
}

/* Creates linked list */
Node * insert_at_head(Node* head, double val)
{
  Node *ptr = Node_construct(val);
  ptr->next = head;
  return ptr;
}

/* Insert at node at tail */
Node* insert_at_tail(Node* head, double val)
{
  Node* ptr = Node_construct(val);
  if (head == NULL)
    return ptr;
  
  Node* p = head;
  while (p->next != NULL)
  {
    p = p->next;
  }
  p->next = ptr;
  return head;
}

/* Inserts node at given position */
Node* insert_at_position(Node* head, double val, int position)
{
	int i = 0;
  Node* ptr = Node_construct(val);
  if (head == NULL)
    return ptr;
  
  Node* p = head;
  while (i != position-1)
  {
    p = p->next;
	i++;
  }
  ptr -> next = p->next;
  p->next = ptr;
  return head;
}

/* Deletes node at given position */
Node* delete_at_position(Node* head, int position)
{
	int i = 0;

  Node* p = head;
  if(position == 0)
  {
	  head = p->next;
	  return head;
  }
  else
  {
	  while (i != position-1)
	  {
		  p = p->next;
		  i++;
	  }
	  Node *next = p ->next;
	  p->next = next->next;
      return head;
  }
}

/* Prints linked list forward */
void print_simple_list(Node *p)
{
  while (p != NULL)
  {
    printf("Node found: %.2f\n", p->value);
    p = p->next;
  }
}

/* Prints list in reverse */
void print_in_reverse(Node * p)
{
  if (p->next == NULL)
  {
    printf("Node found: %.2f\n", p->value);
    return;
  }
  print_in_reverse(p->next);
  printf("Node found: %.2f\n", p->value);
}

/* Frees up memory */
void free_list(Node* p)
{
  if (p == NULL)
  {
    return;
  }
  free_list(p->next);
  free(p);
}

#endif