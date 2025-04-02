#ifndef QUEUE_H_
#define QUEUE_H_

#include "linked_list.h"
#include "tile_game.h"

struct queue {
  struct linked_list data;
};

void enqueue(struct queue *q, struct game_state state);
struct game_state dequeue(struct queue *q);

int number_of_moves(struct game_state start);

// bool is_solved(struct game_state state);
// int generate_next_states(struct game_state current, struct game_state next_states[4]);


#endif