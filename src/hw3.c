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

    result->top = 0;
    result->items[result->top] = result;

    char ch;
    int rows = 0;
    int cols = 0;
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
    
    

    fclose(file);
    return result;
}

GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed) {

    
    GameState *temp_struct = (GameState *)malloc(sizeof(GameState)); //temprorary


    int length = strlen(tiles);
    // printf("%d\n", length);

    // //legal check
    if(row<0 || row>=game->rows){
        return game;
    }else if(col<0 || col>=game->cols){
        return game;
    }else if(direction!='H' && direction!='V'){
        return game;
    }
    int i = 0;
    int j = 0;

    // // - check existence of tile in word.txt

    // int tile_exist = 0;

    // char word[100];
    // char input_word[8];
    // int empty_space = 0;

    // if (direction=='H' || direction=='h'){
    //     for(int i = 0; i<length; i++){
    //         if (tiles[i]==' '){
    //             input_word[i] = game->board[row][col+i];
    //             empty_space=1;
    //         }else{
    //             input_word[i] = tiles[i];
    //         }
    //     }
    // }

    // if (direction=='V' || direction=='v'){
    //     for(int i = 0; i<length; i++){
    //         if (tiles[i]==' '){
    //             input_word[i] = game->board[row+i][col];
    //             empty_space=1;
    //         }else{
    //             input_word[i] = tiles[i];
    //         }
    //     }
        
    // }
    
    

    // FILE *words_file = fopen("./tests/words.txt", "r");

    // while (fscanf(words_file, "%s", word) != EOF) {
    //     // Process each word
    //     if (empty_space==1){
    //         if(strcasecmp(input_word, word) == 0){
    //             tile_exist = 1;
    //         }
    //     }else{
    //         if(strcasecmp(tiles, word) == 0){
    //             tile_exist = 1;
    //         }
    //     }
        
    // }


    // if(tile_exist==0){
    //     printf("here\n");
    //     return game;
    // }

    // fclose(words_file);


    // // - check if it is first word
    // int first_word = 1;

    // for(int i = 0; i<game->rows; i++){
    //     for (int j = 0; j<game->cols; j++){
    //         if (game->stack_tiles[i][j]>0){
    //             first_word = 0;
    //         }
    //     }
    // }

    // if (first_word==1){
    //     if (length<2){
    //         return game;
    //     }
    // }else{

    //     if (direction=='H' || direction=='h'){
    //     int valid1 = 0;
    //     int valid2 = 0;
    //     for (int i = 0; i<length && col+i<game->cols; i++){
    //         if (game->stack_tiles[row][col+i]!=0){
    //             valid1=1;
    //         }
    //         if(game->board[row][col+i]!=tiles[i]){
    //             valid2=1;
    //         }
    //     }


    //     if(valid1==0){
    //         return game;
    //     }else if(valid2==0){
    //         return game;
    //     }
    // }

    // if (direction=='V' || direction=='v'){
    //     int valid1 = 0;
    //     int valid2 = 0;
    //     for (int i = 0; i<length && row+i<game->rows; i++){
    //         if (game->stack_tiles[row+i][col]!=0){
    //             valid1=1;
    //         }
    //         if(game->board[row+i][col]!=tiles[i]){
    //             valid2=1;
    //         }
    //     }

    //     if(valid1==0){
    //         return game;
    //     }else if(valid2==0){
    //         return game;
    //     }
    // }

    // }


    

    
    //board
    if (direction=='H' || direction=='h'){
        temp_struct->rows = game->rows;
        
        if(col+length>game->cols){
            temp_struct->cols = col+length;
        }else{
            temp_struct->cols = game->cols;
        }

    }

    if (direction=='V' || direction=='v'){
        if (row+length>game->rows){
            temp_struct->rows = row+length;
        }else{
            temp_struct->rows = game->rows;
        }

        temp_struct->cols = game->cols;

    
    }

    temp_struct->board = (char **)malloc(temp_struct->rows * sizeof(char *));
    
    for (i = 0; i<temp_struct->rows; i++){
        temp_struct->board[i] = (char *)malloc(temp_struct->cols * sizeof(char));
        for (j = 0; j<temp_struct->cols; j++){
            temp_struct->board[i][j] = '.';
        }
    }


    for(i = 0; i<game->rows; i++){
        for (j = 0; j<game->cols; j++){
            temp_struct->board[i][j] = game->board[i][j];
                
        }
    }

    // stack_tiles
    temp_struct->stack_tiles = (int **)malloc(temp_struct->rows * sizeof(int *));

    for (i = 0; i<temp_struct->rows; i++){
        temp_struct->stack_tiles[i] = (int *)malloc(temp_struct->cols * sizeof(int));
        for (j = 0; j<temp_struct->cols; j++){
            temp_struct->stack_tiles[i][j] = 0;
        }
    }

    for(i = 0; i<game->rows; i++){
        for (j = 0; j<game->cols; j++){
            temp_struct->stack_tiles[i][j] = game->stack_tiles[i][j];
            
        }   
    }

    //stack allocation
    temp_struct->top = game->top;
    temp_struct->items[temp_struct->top] = game;

    int placed = 0;

    //Horizontal
    if (direction=='H' || direction=='h'){ 
        
        for(i = 0; i<length; i++){
            if(tiles[i]==' '){
                continue;
            }else{
                temp_struct->board[row][col+i] = tiles[i];
                temp_struct->stack_tiles[row][col+i]++;
                placed++;
            }
            
        }
    }

    //Vertical
    if (direction=='V' || direction=='v'){ 

        for(i = 0; i<length; i++){
            if(tiles[i]==' '){
                continue;
            }else{
                temp_struct->board[row+i][col] = tiles[i];
                temp_struct->stack_tiles[row+i][col]++;
                placed++;
            }
            
        }
    }
    
    *num_tiles_placed = placed;

    
    game = temp_struct;
    
    

    game->top++;
    game->items[game->top] = game;
    

    

    return game;
}

GameState* undo_place_tiles(GameState *game) {

    if (game->top==0){
        return game->items[0];
    }
    
    return game->items[--game->top];
}

void free_game_state(GameState *game) {
    while(game->top != 0) {
        game = undo_place_tiles(game);
    }
    
    for (int i = 0; i < game->rows; i++) {
        free(game->board[i]);
    }
    free(game->board);
        
    for (int i = 0; i < game->rows; i++) {
        free(game->stack_tiles[i]);
    }
    free(game->stack_tiles);

    free(game);
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

