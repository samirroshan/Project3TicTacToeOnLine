#ifndef _PLAYER_H_
    
    #define _PLAYER_H_

    #include "utils.h"
    #include "game.h"

    typedef struct sockaddr SA;
    typedef struct sockaddr_in SA_IN;
    
    typedef struct {
        char role; 
        char* name; 
        SA_IN address; 
        int socket; 
        pid_t pid; 
        (struct Game)* game; 
    } Player;

    Player* new_player(char role, char *name, SA_IN address, int socket, pid_t pid);
    void free_player(Player* player);
    int is_player(Player* player, char *name, SA_IN address);

#endif 