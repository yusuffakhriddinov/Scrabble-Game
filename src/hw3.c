#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hw3.h" 

#define DEBUG(...) fprintf(stderr, "[          ] [ DEBUG ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, " -- %s()\n", __func__)

GameState* initialize_game_state(const char *filename) {
    
    FILE *file = fopen(filename, "r");

    GameState *result = (GameState *)malloc(sizeof(GameState));

    char ch;
    int rows;
    int cols;
    while((ch = fgetc(file)) != EOF){
        if (ch == '\n') {
            rows++;
            
        }else if(rows<1){
            cols++;
        }
    }
    
    rewind(file);

    result->rows = rows;
    result->cols = cols; 

    result->board = (char **)malloc(rows * sizeof(char *));
    result->stack_tiles = (int **)malloc(rows * sizeof(int *));

    for (int i = 0; i<rows; i++){
        result->board[i] = (char *)malloc(cols * sizeof(char));
        result->stack_tiles[i] = (int *)malloc(cols * sizeof(int));
        for (int j = 0; j<cols; j++){
            char temp = fgetc(file);
            result->board[i][j] = temp;
            if (temp=='.'){
                result->stack_tiles[i][j] = 0;
            }else{
                result->stack_tiles[i][j] = 1;
            }
            
            
        }
        fgetc(file);
    }
    
    printf("%p\n", (void *)result);
    

    fclose(file);
    return result;
}

GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed) {
    (void)game;
    (void)row;
    (void)col;
    (void)direction;
    (void)tiles;
    (void)num_tiles_placed;

    // int placed = 0;
    GameState *temp_struct = game+1;

    int length = strlen(tiles);

    //invalid parameters
    if(row<0 || row>=game->rows){
        return game;
    }else if(col<0 || col>=game->cols){
        return game;
    }else if(direction!='H' && direction!='V'){
        return game;
    }else if(row+length>game->rows || col+length>game->cols){
        return game;
    }


    //Horizontal
    if (direction=='H' || direction=='h'){ 
        
        for(int i = 0; i<length; i++){
            if(tiles[i]==' '){
                continue;
            }else{
                game->board[row][col+i] = tiles[i];
                game->stack_tiles[row][col+i]++;
                // placed++;
            }
            
        }
    }

    //Vertical
    if (direction=='V' || direction=='v'){ 
        
        for(int i = 0; i<length; i++){
            if(tiles[i]==' '){
                continue;
            }else{
                game->board[row+i][col] = tiles[i];
                game->stack_tiles[row+i][col]++;
                // placed++;
            }
            
        }
    }

    
    temp_struct->rows = game->rows;
    temp_struct->cols = game->cols;

    //board
    temp_struct->board = (char **)malloc(temp_struct->rows * sizeof(char *));

    for(int i = 0; i<temp_struct->rows; i++){
        temp_struct->board[i] = (char *)malloc(temp_struct->cols * sizeof(char));
        for (int j = 0; j<temp_struct->cols; j++){
            temp_struct->board[i][j] = game->board[i][j];
            
        }
       
    }

    //stack_tiles
    temp_struct->stack_tiles = (int **)malloc(temp_struct->rows * sizeof(int *));

    for(int i = 0; i<temp_struct->rows; i++){
        temp_struct->stack_tiles[i] = (int *)malloc(temp_struct->cols * sizeof(int));
        for (int j = 0; j<temp_struct->cols; j++){
            temp_struct->stack_tiles[i][j] = game->stack_tiles[i][j];
            
        }
       
    }

    

    // num_tiles_placed = placed;
    
    game = temp_struct;
    printf("%p\n", (void *)game);
    return game;
}

GameState* undo_place_tiles(GameState *game) {
    (void)game;
    game--;
    // printf("%p\n", (void *)game);
    return game;
}

void free_game_state(GameState *game) {
    (void)game;
}

void save_game_state(GameState *game, const char *filename) {
    

    FILE *file = fopen(filename, "w");

    for (int i = 0; i<game->rows; i++){
        for (int j = 0; j<game->cols; j++){
            fprintf(file, "%c", game->board[i][j]);
        }
        fprintf(file, "\n");
    }

    for (int i = 0; i<game->rows; i++){
        for (int j = 0; j<game->cols; j++){
            fprintf(file, "%d", game->stack_tiles[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

}

