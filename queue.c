#include "queue.h"
#include "tile_game.h"
#include <stdbool.h>
#include <string.h>

// Enqueue a game state into the queue
void enqueue(struct queue *q, struct game_state state) {
    size_t serialized_state = serialize(state);
    insert_at_tail(&q->data, serialized_state);
}

// Dequeue a game state from the queue
struct game_state dequeue(struct queue *q) {
    if (!q->data.head) {
        struct game_state empty = {0};
        return empty; // Return zeroed state for empty queue
    }
    size_t serialized_state = remove_from_head(&q->data);
    return deserialize(serialized_state);
}

// Perform BFS to find the shortest number of moves
int number_of_moves(struct game_state start) {
    struct queue q = {{NULL}}; // Properly initialize queue

    // Visited set to prevent revisiting states
    #define MAX_VISITED 1000000
    static uint64_t visited[MAX_VISITED] = {0};
    static int visited_count = 0;
    memset(visited, 0, sizeof(visited));
    visited_count = 0;

    enqueue(&q, start);
    visited[visited_count++] = serialize(start);

    while (q.data.head) {
        struct game_state current = dequeue(&q);

        // Inline check for solved state
        uint8_t solved[4][4] = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0}
        };
        if (memcmp(current.tiles, solved, sizeof(solved)) == 0) {
            free_list(q.data);
            return current.num_steps;
        }

        // Inline generation of next states using tile_game.c functions
        struct game_state next_states[4];
        int num_next = 0;

        struct game_state temp = current;
        move_up(&temp);
        if (memcmp(temp.tiles, current.tiles, sizeof(temp.tiles)) != 0) {
            next_states[num_next++] = temp;
        }

        temp = current;
        move_down(&temp);
        if (memcmp(temp.tiles, current.tiles, sizeof(temp.tiles)) != 0) {
            next_states[num_next++] = temp;
        }

        temp = current;
        move_left(&temp);
        if (memcmp(temp.tiles, current.tiles, sizeof(temp.tiles)) != 0) {
            next_states[num_next++] = temp;
        }

        temp = current;
        move_right(&temp);
        if (memcmp(temp.tiles, current.tiles, sizeof(temp.tiles)) != 0) {
            next_states[num_next++] = temp;
        }

        // Enqueue unvisited next states
        for (int i = 0; i < num_next; i++) {
            uint64_t next_serialized = serialize(next_states[i]);
            bool is_visited = false;
            for (int j = 0; j < visited_count; j++) {
                if (visited[j] == next_serialized) {
                    is_visited = true;
                    break;
                }
            }
            if (!is_visited && visited_count < MAX_VISITED) {
                enqueue(&q, next_states[i]);
                visited[visited_count++] = next_serialized;
            }
        }
    }

    free_list(q.data);
    return -1; // No solution found
}