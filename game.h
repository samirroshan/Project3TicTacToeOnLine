#ifndef _GAME_H_

    #define _GAME_H_

    #include "utils.h"
    #include "player.h"

    typedef struct {
        char* board; 
        int turn;

        pthread_mutex_t lock;
        Player* playerX; 
        Player* playerO;
    } Game;

    Game* new_game(); 
    void free_game(Game* game); 
    void draw(Game* game, Player* player); 
    void move(Game* game, Player* player, int x, int y); 
    void resign(Game* game, Player* player); 
    int check_game(Game* game); 
    void print_board_as_display(Game* game); 

#endif 