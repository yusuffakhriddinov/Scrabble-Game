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
    int placed = 0;
    // printf("%d\n", length);
    
    // //legal check
    if(row<0 || row>game->rows){
        *num_tiles_placed = placed;
        return game;
    }else if(col<0 || col>game->cols){
        *num_tiles_placed = placed;
        return game;
    }else if(direction!='H' && direction!='V'){
        *num_tiles_placed = placed;
        return game;
    }
    int i = 0;
    int j = 0;

    

    // - check existence of tile in word.txt

    int tile_exist = 0;

    char word[100] = {0};  
    char input_word[100] = {0}; 
    int empty_space = 0;
    (void) empty_space;

    if (direction == 'H' || direction == 'h') {

        int l = 0;
        i=1;
        int m = 0;
        char temp_text[100] ={0};
         while (i <= col) {
            if (game->board[row][col-i] != '.') { // dropped half
                temp_text[m] = game->board[row][col-i];
            }else{
                break;
            }
            i++;
            m++;
            
        }
        for(i = strlen(temp_text)-1; i>=0; i--){
            input_word[l] = temp_text[i];
            l++;
        }
        
        int k = 0;
        int n = 0;
        for (k = 0; k < length; k++) {
            if (tiles[n] == ' ') {
                if(game->board[row][col + n]=='.'){
                    *num_tiles_placed = placed;
                    return game;
                }
                input_word[l] = game->board[row][col + n];
                empty_space = 1;
            } else {
                input_word[l] = tiles[n];
            }
            n++;
            l++;
            
        }

        
        
        while (col + n < game->cols && game->board[row][col + n] != '.') {
            input_word[l] = game->board[row][col + n];
            n++;
            l++;
        }
        
    }

    if (direction=='V' || direction=='v'){
        int l = 0;
        i=1;
        int m = 0;
        char temp_text[100] ={0};
         while (i <= row) {
            if (game->board[row-i][col] != '.') { // dropped half
                temp_text[m] = game->board[row-i][col];
            }else{
                break;
            }
            i++;
            m++;
            
            
            // if ((game->board[i][col] != '.' && game->board[i+1][col]!='.') || i+1==row) { // dropped half
            //     input_word[l] = game->board[i][col];
            //     l++;
            // }
            // i++;
        }
        
        for(i = strlen(temp_text)-1; i>=0; i--){
            input_word[l] = temp_text[i];
            l++;
        }

        int k = 0;
        int n = 0;
        for (k = 0; k < length; k++) {
            if (tiles[n] == ' ') {
                if(game->board[row + n][col]=='.'){
                    *num_tiles_placed = placed;
                    return game;
                }
                input_word[l] = game->board[row + n][col];
                
                empty_space = 1;
            } else {
                input_word[l] = tiles[n];
            }
        
            n++;
            l++;
        }
        
        
        while (row + n<game->rows && game->board[row + n][col]!='.' ){ 
            input_word[l] = game->board[row+n][col];
            n++;
            l++;
        }

    }
    
    char temp_word[100] ={0};
    if (input_word[0]=='.'){
        for (i = 0, j = 1; j<=length; j++){
            temp_word[i] = input_word[j];
            i++;
        }
        strcpy(input_word, temp_word);
    }
    
    FILE *words_file = fopen("./tests/words.txt", "r");

    while (fscanf(words_file, "%s", word) != EOF) {
        if(strcasecmp(input_word, word) == 0){
            tile_exist = 1;
        }
    }

    
    if(tile_exist==0){
        *num_tiles_placed = placed;
        return game;
    }

    fclose(words_file);


    // - check if it is first word
    int first_word = 1;

    for(i = 0; i<game->rows; i++){
        for (j = 0; j<game->cols; j++){
            if (game->stack_tiles[i][j]>0){
                first_word = 0;
            }
        }
    }

    if (first_word==1){
        if (length<2){
            
            *num_tiles_placed = placed;
            return game;
        }
    }else{
        if (direction=='H' || direction=='h'){
            int valid1 = 0;
            int valid2 = 0;
            i = 0;

            if (col>0 && game->stack_tiles[row][col-1]!=0){ //col-1>game->cols
                valid1=1;
            }

            for (i = 0; i<length && col+i<game->cols; i++){
                if (game->stack_tiles[row][col+i]!=0){
                    valid1=1;
                }
                if(game->board[row][col+i]!=tiles[i]){
                    valid2=1;
                }
            }

            if (col+i<game->cols && game->stack_tiles[row][col+i]!=0){
                valid1=1;
            }

            if(valid1==0){
                *num_tiles_placed = placed;
                return game;
            }else if(valid2==0){
                
                *num_tiles_placed = placed;
                return game;
            }
        }

        if (direction=='V' || direction=='v'){
            int valid1 = 0;
            int valid2 = 0;
            i = 0;

            if (row>0 && game->stack_tiles[row-1][col]!=0){//!=
            
                valid1=1;
            }

            for (i = 0; i<length && row+i<game->rows; i++){
                if (game->stack_tiles[row+i][col]!=0){
                    
                    valid1=1;
                }
                if(game->board[row+i][col]!=tiles[i]){
                    valid2=1;
                }
            }
            
            if (row+i<game->rows && game->stack_tiles[row+i][col]!=0){
                
                valid1=1;
            }

            if(valid1==0){
                
                *num_tiles_placed = placed;
                return game;
            }else if(valid2==0){
                *num_tiles_placed = placed;
                return game;
            }
        }

    }

    
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

    // full covering check
    if (direction=='H'){
        int complete_pos = 1;
        if ((int)strlen(input_word) == length && length>1){
            for (int r = 0; r < length; r++){
                if(temp_struct->board[row][col+r] == '.' ){
                    complete_pos = 0;
                    break;
                }
                if (tiles[r]==' '){
                    complete_pos = 0;
                    break;
                }
            }
            
            if (complete_pos==1){
                *num_tiles_placed = 0;
                return game;
            }
        }

    }
    if (direction=='V'){
        int complete_pos = 1;
        if ((int)strlen(input_word) == length){
            for (int r = 0; r < length; r++){
                if((int)strlen(input_word) == length || temp_struct->board[row+r][col] == '.'){
                    complete_pos = 0;
                }
            }
            if (complete_pos==1 && length>1){
                *num_tiles_placed = 0;
                return game;
            }
        }

    }


    

    //Horizontal
    if (direction=='H' || direction=='h'){    
        for(i = 0; i<length; i++){
            if(tiles[i]==' '){
                continue;
            }else{
                if (tiles[i]==temp_struct->board[row][col+i]){
                    *num_tiles_placed = 0;
                    printf("%d", *num_tiles_placed);
                    return game;
                }
                temp_struct->board[row][col+i] = tiles[i];

                if(temp_struct->stack_tiles[row][col+i]>=5){
                    *num_tiles_placed = 0;
                    return game;
                }else{
                    temp_struct->stack_tiles[row][col+i]++;
                }
                
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
                if (tiles[i]==temp_struct->board[row+i][col]){
                    *num_tiles_placed = 0;
                    return game;
                }
                temp_struct->board[row+i][col] = tiles[i];

                if (temp_struct->stack_tiles[row+i][col]>=5){
                    *num_tiles_placed = 0;
                    return game;
                }else{
                    temp_struct->stack_tiles[row+i][col]++;
                }
                
                placed++;
            } 
        }
    }

    // Bottomline letter change check

    // for (int r = 0; r < length; r++) {
    //     int temp_row = row;
    //     int temp_col = col + r;
    //     char temp_input_word[100] = {0}; // Initialize temp_input_word

    //     if (temp_row + 1 < temp_struct->rows && temp_struct->board[temp_row + 1][temp_col] == '.' &&
    //         temp_row - 1 >= 0 && temp_struct->board[temp_row - 1][temp_col] != '.') {

    //         int v = 0;
    //         int h = 0;
            
    //         // Iterate until v is less than the number of rows in temp_struct->board
    //         while (v <= temp_row) {
    //             if (temp_struct->board[v][temp_col] != '.') {
    //                 temp_input_word[h] = temp_struct->board[v][temp_col];
    //                 h++;
    //             }
    //             v++;
    //         }

    //         // Process temp_input_word as needed
    //         // For example, print it:
            
    //     }
    //     int temp_tile_exist = 0;
    //     FILE *temp_words_file = fopen("./tests/words.txt", "r");

    //     while (fscanf(temp_words_file, "%s", word) != EOF) {
    //         if(strcasecmp(temp_input_word, word) == 0){
    //             temp_tile_exist = 1;
    //         }
    //     }

        
    //     if(temp_tile_exist==0){
    //         *num_tiles_placed = 0;
    //         return game;
    //     }

    //     fclose(temp_words_file);

    // }
    
    
    *num_tiles_placed = placed;

    
    game = temp_struct;
    
    

    game->top++;
    game->items[game->top] = game;
    
    
    return game;
}

GameState* undo_place_tiles(GameState *game) {
    if (game->top == 0) {
        return game->items[0];
    }
    GameState *previous_state = game->items[game->top - 1];
    
    // Free board and stack_tiles for the current state
    for (int i = 0; i < game->rows; i++) {
        free(game->board[i]);
        free(game->stack_tiles[i]);
    }
    free(game->board);
    free(game->stack_tiles);
    
    // Free the current state itself
    free(game);
    
    return previous_state;
}

void free_game_state(GameState *game) {
    while (game->top != 0) {
        GameState *previous_state = undo_place_tiles(game);
        

        // for (int i = 0; i < game->rows; i++) {
        //     free(game->board[i]);
        //     free(game->stack_tiles[i]);
        // }
        // free(game->board);
        // free(game->stack_tiles);
        // free(game);
        
        // Move to the next state
        game = previous_state;
    }

    // Free the memory associated with the final state
    for (int i = 0; i < game->rows; i++) {
        free(game->board[i]);
        free(game->stack_tiles[i]);
    }
    free(game->board);
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

