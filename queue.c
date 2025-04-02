#include "queue.h"
#include "tile_game.h"



// Function prototypes (ensure they're implemented somewhere)
bool is_solved(struct game_state state);
int generate_next_states(struct game_state current, struct game_state next_states[4]);

// Enqueue a game state into the queue
void enqueue(struct queue *q, struct game_state state) 
{
    size_t serialized_state = serialize(state);
    insert_at_tail(&q->data, serialized_state);
}

// Dequeue a game state from the queue
struct game_state dequeue(struct queue *q) 
{
    if (!q->data.head) 
    {
        return (struct game_state){0}; // Return an empty state if queue is empty
    }
    size_t serialized_state = remove_from_head(&q->data);
    return deserialize(serialized_state);
}

// Perform BFS to find the shortest number of moves
int number_of_moves(struct game_state start) 
{
    struct queue q;
    q.data.head = NULL; // Initialize queue properly

    enqueue(&q, start);

    while (q.data.head) 
    {
        struct game_state current = dequeue(&q);

        if (is_solved(current)) 
        {
            free_list(q.data);
            return current.num_steps; // Use num_steps instead of num_moves
        }

        struct game_state next_states[4];
        int num_next = generate_next_states(current, next_states);

        for (int i = 0; i < num_next; i++) 
        {
            enqueue(&q, next_states[i]);
        }
    }

    free_list(q.data);
    return -1; // No solution found (should not happen for a valid game state)
}