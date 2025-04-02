#include "linked_list.h"

#include <stdlib.h>

// Create a new node with the given value
struct list_node *new_node(size_t value) 
{
  struct list_node *node = (struct list_node *)malloc(sizeof(struct list_node));
  if (node) 
  {
    node->value = value;
    node->next = NULL;
  }
  return node;
}

// Insert a value at the head of the list
void insert_at_head(struct linked_list *list, size_t value) 
{
  struct list_node *node = new_node(value);
  if (!node) return;
  node->next = list->head;
  list->head = node;
}

// Insert a value at the tail of the list
void insert_at_tail(struct linked_list *list, size_t value) 
{
  struct list_node *node = new_node(value);
  if (!node) return;
  if (!list->head) 
  {
    list->head = node;
    return;
  }
  struct list_node *cur = list->head;
  while (cur->next) 
  {
    cur = cur->next;
  }
  cur->next = node;
}

// Remove a value from the head of the list
size_t remove_from_head(struct linked_list *list) 
{
  if (!list->head) return 0;
  struct list_node *temp = list->head;
  size_t value = temp->value;
  list->head = temp->next;
  free(temp);
  return value;
}

// Remove a value from the tail of the list
size_t remove_from_tail(struct linked_list *list) 
{
  if (!list->head) return 0;
  struct list_node *cur = list->head;
  struct list_node *prev = NULL;
  while (cur->next) 
  {
    prev = cur;
    cur = cur->next;
  }
  size_t value = cur->value;
  free(cur);
  if (prev) 
  {
    prev->next = NULL;
  } else 
  {
    list->head = NULL;
  }
  return value;
}

// Free all nodes in the list
void free_list(struct linked_list list) 
{
  struct list_node *cur = list.head;
  while (cur) 
  {
    struct list_node *next = cur->next;
    free(cur);
    cur = next;
  }
}


// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) 
{
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) 
  {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
