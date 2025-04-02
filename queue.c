#include "queue.h"
#include "tile_game.h"

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

// Check if a game state is solved
bool is_solved(struct game_state state) 
{
    struct game_state solved_state = {
        .tiles = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0}
        },
        .empty_row = 3,
        .empty_col = 3,
        .num_steps = 0
    };

    return serialize(state) == serialize(solved_state);
}

// Generate next possible states
int generate_next_states(struct game_state current, struct game_state next_states[4]) 
{
    int num_next = 0;

    // Try moving the empty tile in all four directions
    if (current.empty_row > 0) { // Can move up
        struct game_state next = current;
        move_up(&next);
        next_states[num_next++] = next;
    }

    if (current.empty_row < 3) { // Can move down
        struct game_state next = current;
        move_down(&next);
        next_states[num_next++] = next;
    }

    if (current.empty_col > 0) { // Can move left
        struct game_state next = current;
        move_left(&next);
        next_states[num_next++] = next;
    }

    if (current.empty_col < 3) { // Can move right
        struct game_state next = current;
        move_right(&next);
        next_states[num_next++] = next;
    }

    return num_next;
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
            return current.num_steps; // Return the number of steps (moves) taken
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
