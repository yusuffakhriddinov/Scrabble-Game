#include <stdlib.h>

typedef struct GameState
{
    int rows;
    int cols;
    char** board;
    int** stack_tiles;

    //stack for memory
    void *items[100]; // check it later
    int top; 
    
} GameState;


GameState* initialize_game_state(const char *filename);
GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed);
GameState* undo_place_tiles(GameState *game);
void free_game_state(GameState *game);
void save_game_state(GameState *game, const char *filename);
