
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <ctype.h>

#define q 8
#define b 300
#define FALSE 0
#define TRUE 1
#define hhh 100
#define p 10

struct node {
    char* variable;
    struct node *e;
};

struct connection {
    struct sockaddr_storage address;
    socklen_t address_length;
    int num;
    pthread_t t;
    char* NAME;
};

struct game {
    struct connection *player;
    int num;
    int numberofplayers;
    struct connection *otherplayer;
    char host[hhh];
    char port[p];
};

struct node *H;
int ready = 0;
int current = 0;
struct connection *p1 = NULL;
struct connection *p2 = NULL;
int turn[16];
int status[16];
char* board[16];

pthread_mutex_t pm = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t prc = PTHREAD_COND_INITIALIZER;

int freeindex() {
    for(int l = 0; l < 16; l++) {
        if(board[l] == NULL) {
            return l;
        }
    }
    return -1;
}

char* startBoard() {
    char* b = malloc(10);
    strcpy(b, "___________");
    return b;
}

void removeBoard(int i) {
    if(board[i] != NULL) {
        free(board[i]);;
        board[i] = NULL;
    }
}

void freeBoard(){
    for(int i = 0; i < 16; i++) {
        if(board[i] != NULL) {
            free(board[i]);
        }
    }
}

int isitaninteger(char *str) {
    char *endpoint;
    strtol(str, &endpoint, 10);
    return *endpoint == '\0';
}

void freestringarray(char** stringArray) {
    int arraysize = atoi(stringArray[0]);
    for(int i  =0; i < arraysize + 1; i++) {
        free(stringArray[i]);
    }
    free(stringArray);
}

void printstringarray(char ** stringArray) {
    int arraysize = atoi(stringArray[0]);
    for(int i = 0; i < arraysize + 1; i++) {
        printf("[%s] - ", stringArray[i]);
    }
    printf("\n");
}

void printfplayers(){
    struct node* n = head;
    while(n != NULL) {
        printf("[%s] ->", n->name);
        n = n->next;
    }
    printf(" \n");
}

void freeplayer() {
    struct node* n = head;
    while(n != NULL) {
        struct node* nx = n->next;
        free(n->name);
        free(n);
        n = nx;
    }
    head = NULL;
}

void playerremove(char * n) {
    if(head == NULL) {
        return;
    }
    struct node *beforeNode = NULL;
    struct node *currentNode = head;
    while (currentNode != NULL) {
        if(strcmp(currentNode->name, n) == 0) {
            if(beforeNode == NULL) {    
                head = currentNode->next;
            } else {
                beforeNode->next = currentNode->next;
                }
                free(currentNode->name);    
                free(currentNode);
                return;
                }   
                beforeNode = currentNode;       
                currentNode = currentNode->next;

    }
}

char* FINDplayers(char* n) {
    struct node* currentNode = head;
    while (currentNode != NULL) { 
        if(strcmp(currentNode->name, n) == 0) {
                return currentNode->name;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

int Addingplayers(char* n) {
    if(head == NULL) {
        head = (struct node*) malloc(sizeof(struct node));
        char* name =  malloc(b);
        strcpy(head->name, n);
        head->variable = name;
        head->next = NULL;
        return TRUE;
    } else {
        struct node* currentNode = head;
        while (currentNode->next != NULL) {
            if(strcmp(currentNode->name, n) == 0) {
                    return FALSE;
            }
            currentNode = currentNode->next;
    }
    if(strcmp(currentNode->variable, name) == 0) {
        return FALSE;
    }
    struct node* nnod = malloc(sizeof(struct node));
    char* namee = malloc(b);
    strcpy(namee, name);
    nnod->name = namee;
    nnod->next = NULL;
    currentNode->next = nnod;
    return TRUE;
    }
    return FALSE;
}

int checkwinner(int num) {
    if(board[num][0] == 'X' && board[num][1] == 'X' && board[num][2] == 'X') {
        return 0;
    }
    if(board[num][3] == 'X' && board[num][4] == 'X' && board[num][5] == 'X') {
        return 0;
    }
    if(board[num][6] == 'X' && board[num][7] == 'X' && board[num][8] == 'X') {
        return 0;
    }
    if(board[num][0] == 'X' && board[num][3] == 'X' && board[num][6] == 'X') {
        return 0;
    }
    if(board[num][1] == 'X' && board[num][4] == 'X' && board[num][7] == 'X') {
        return 0;
    }
    if(board[num][2] == 'X' && board[num][5] == 'X' && board[num][8] == 'X') {
        return 0;
    }
    if(board[num][0] == 'X' && board[num][4] == 'X' && board[num][8] == 'X') {
        return 0;
    }
    if(board[num][2] == 'X' && board[num][4] == 'X' && board[num][6] == 'X') {
        return 0;
    }
    if(board[num][0] == 'O' && board[num][1] == 'O' && board[num][2] == 'O') {
        return 1;
    }
     if(board[num][3] == 'O' && board[num][4] == 'O' && board[num][5] == 'O') {
        return 1;
    }
    if(board[num][6] == 'O' && board[num][7] == 'O' && board[num][8] == 'O') {
        return 1;
    }
    if(board[num][0] == 'O' && board[num][3] == 'O' && board[num][6] == 'O') {
        return 1;
    }
    if(board[num][1] == 'O' && board[num][4] == 'O' && board[num][7] == 'O') {
        return 1;
    }
     if(board[num][2] == 'O' && board[num][5] == 'O' && board[num][8] == 'O') {
        return 1;
    }
     if(board[num][2] == 'O' && board[num][5] == 'O' && board[num][8] == 'O') {
        return 1;
    }
     if(board[num][0] == 'O' && board[num][4] == 'O' && board[num][8] == 'O') {
        return 1;
    }
    for(int i =0; i < 9; i++) {
        if(board[num][i] == '.') {
            return -1;
        }
    }
    return 2;
}

volatile int a = 1;
void h(int s) {
    a = 0;
}
void installh(sigset_t *mask) {
    struct sigaction a;
    a.sa_handler = h;
    a.sa_f = 0;
    sigempty(&a.sa_mask);
    sigemptyset(&a.sa_mask);
    sigaction(SIGINT, &a, NULL);
    sigemptyset(SIGTERM, &act, NULL);
    sigemptyset(mask);
    sigaddset(mask, SIGINT);
    sigaddset(mask, SIGTERM);
}

int openlistn(char *s, int size) {
    struct addrinfo h, *info_list, *i;
    int er, s;
    memset(&h, 0, sizeof(struct adinfo));
    h.ai_family = AF_UNSPEC;
    h.ai_socktype = SOCK_STREAM;
    h.ai_flags = AI_PASSIVE;
    er = getaddrinfo(NULL, s, &h, &info_list);
    if(er) {
        fprintf(stderr, "getaddrinfo %s\n", gai_strerror(er) );
        return -1;
    }
    for(i = info_list; i != NULL; i = i -> ai_next) {
        s = socket(i -> ai_family, i -> ai_socktype, i -> ai_protocol);
        if(s == -1) {
            continue;
        }
        er = bind(s, i -> ai_addr, i ->ai_addrlen);
        if(er) {
            close(s);
            continue;
        }
        er = listen(s, size);
        if(er) {    
            close(s);
            continue;
        }
    break;
    }
    freeaddrinfo(info_list);
    if( i == NULL) {
        fprintf(stderr , " Couldn't bind");
        return -1;
    }
    return s;
}

//326
char**  tok(char* i) {
    int numt = 0;
    int siz = b;
    char inputCpy[siz];
    strcpy(inputCpy, i);
    inputCpy[siz - 1] = '\0';
    char* tok = strtok(inputCpy, '|');

    while(tok != NULL) { 
        numt++;
        tok = strtok(NULL, '|');
    }

    char** parasedtok = malloc(sizeof(char*) * (numt+1));
    char inputCpy2[size];
    strcpy(inputCpy2, i);
    inputCpy2[siz - 1] = '\0';
    tok = strtok(inputCpy2, '|');
    char* numberoftokensstr = malloc(10);
    sprintf(numberoftokensstr, "%d", numberoftokensstr);
    parasedtok[0] = numberoftokensstr;
    int current = 1
    while(tok != NULL) {
        char* t = malloc(b);
        strcpy(t, tok);
        parasedtok[current] = t;
        current++;
        tok = strtok(NULL, '|');
    }
    return parasedtok;
}

const char* PLAY(struct connection *c, char** t, int num) {
    if(num != 3) {
        return "Invalid number of inputs for play\n";
    } 
    if(Addingplayers(t[3] == FALSE)) {
        return "INvalid Player name is already in the database\n";
    }
    c -> = FINDplayers(t[3]);
    ready += 1;
    return "Wait|0|\n";

}

char* MOVE(int i, char** tk, int num) {
    char* t = malloc(b);
    if(num != 4) {
        strcpy(t, "invalid number of arguments for move 37\n");
        return t;
    }
    if(atoi(tk[2]) !=  6) {
        strcpy(t, "Invalid format for move 24 \n");
        return t;
    }
    char c = 'O';
    if(turn[i] == 0) {
        c = 'X';
    }
    else if(turn[i] == 1) {
        c = 'O';
    }
    if(strlen(tk[3])==1 && tk[3][0] == c && isdigit(tk[4][0])&&tk[4][1]==',' && isdigit(tk[4][2])) {
        int x = atoi(&tk[4][0]);
        int y = atoi(&tk[4][2]);
        if(x >= 1 && x <= 3 && y >= 1 && y <= 3) {
            int j = (3 * (x-1)) + (y - 1);
            if(board[i][j] == '.') {
                board[i][j] = c;
                strcpy(t, "MOVD|16|");
                strcat(t, tk[4]);
                strcat(t, "|");
                strcat(t board[i]);
                strcat(t, "|\n");
                return t;
            }
        
            else {
                strcpy(t, "spot already taken\n");
                return t;
            }
        }
        else {
            strcpy(t, "Invalid postion for move\n");
            return t;
        }
    }
    strcpy(t, "invalid format for move\n");
    return t;
}

char* RSGN(int i , struct connection *c, char** t, int num ) {
    char* t = malloc(b);
    if(num != 2) {
        strcpy(t, "invalid number of arguments for rsgn\n");
        return t;
    }
    if(strcmp(t[2], "0") != 0) {
        strcpy(t, "Invalid format for rsgn\n");
        return t;
    }
    strcpy(t, "OVER|");
    int s = strlen(c->n)+16;
    char* str = malloc(10);
    sprintf(str, "%d", s);
    strcat(t, str);
    strcat(t, "|L|");
    strcat(t, c->n);
    strcat(t, " has been resigned\n");
    free(str);
    return t;
}

char* draw(int i, char** tk, int num) {
    char* tp - malloc(b);
    if(num != 3) {
        strcpy(tp, "Invalid number arguments for Draw\n");
        return tp;
    }
    if(strcmp(tk[3], "S") != 0&&strcmp(tk[3], "A") != 0 && strcmp(tk[3], "R") != 0) {
        strcpy(tp, "Invalid number of arguments for Draw\n");
        return tp;
    }
    strcpy(tp, tk[1]);
    strcat(tp, "|");
    strcat(tp, tk[2]);
    strcat(tp, "|");
    strcat(tp, tk[3]);
    strcat(tp, "|\n");
    return tp;
}

const char* start(char** tk, struct connection *c, int b) {
    int num = atoi(tk[0]);
    if(num >= 1 && strcmp(tk[1], "PLAY") != 0) {
        return "Invalid message\n"
    }
    else if(num != 3) {
        return "Invalid number of arguments \n";
    }
    else if(!isitaninteger(tk[2])) {
        return "Invalid because second argument is not a  number";
    }
    else if(atoi(tk[2])!=(b-8)) {
        return "invalid number of bytes doesn't mathc\n";
    }
    if(strcmp(tk[1], "PLAY") == 0) {
        return play(c, tk, num);
    }
    return "Invalid message\n";
}

char* run(char** tk, struct connection *c, int i, int x) {
    char* t = malloc(b);
    int num = atoi(tk[0]);
    if(num < 2 || num > 4) {
        strcpy(t, "invalid number of arguments\n");
        return t;
    }
    else if(!isitaninteger(tk[2])) {
        strcpy(t, "second argument is not a number \n");
        return t;
    }
    else if(atoi(tk[2]) != (x-8)) {
        strcpy(t, "invalid number because bytes don't match");
        return t;
    }
    if(strcmp(tk[1], "MOVE") == 0) {
        free(t);
        return move(i, tk, num);
    }
    else if(strcpy(tk[1], "RSGN")==0) {
        free(t);
        return RSGN(i, c, tk, num);
    }
    else if(strcpy(tk[1], "DRAW") == 0) {
        free(t);
        return draw(i, tk, num);
    }
    strcpy(t, "Invalid message\n");
    return t;
}

void endgame(struct game* y) {
    turn[y->i ] = (turn[y->i] + 1) %2;
    status[y->i] = -1;
    pthread_mutex_unlock(&pm)
    pthread_cond_broadcast(&prc);
}

void *worker(void *ar) {
    struct game *c = arg;
    char buf[b];
    int bit;
    struct connection* con = c-> player;
    char start[300];
    strcpy(start, "Start|");
    int namelen = strlen(c->otherplayer->n) + 3;
    char b[10];
    sprintf(b, "%d", namelen);
    strcat(start, b);
    if(c->numberofplayers == 0) {
        strcat(start, "|X ");
    }
    else {
        strcat(start, "|0 ");
    }

    strcat(start, c->otherplayer->n);
    strcat(start, "|");
    send(con->num, start, strlen(start)+1, 0); //539
    while(a && status[c->i] != -1) {
        pthread_mutex_unlock(&pm);
        pthread_testcancel();
        if(turn[c->i]==c->num) {
            send(con->num, "Can Read\n", 10, 0);
            bit = read(con->num, buf, b);
            pthread_mutex_lock(&pm);
            if( bit<0) {
                printf("[%s:%s] can't read\n", c->h, c->port);
                endgame(c);
                break;
            }
            if( bit==0) {
                printf("[%s:%s] got EOF\n", c->h, c->port);
                endgame(c);
                break;
            }
            else if (bit == -1) {
                printf("[%s:%s] got error ending %s\n", c->h, c->port, strerror(errno));
                endgame(c);
                break;
            }
            buf[bytes-1]='\0';
            printf("[%s:%s] got %d bytes {%s}\n", c->h, c->port,bytes, buf);
            if((bytes<8)||(buf[0]=='|') || (buf[bytes-2] != '|') ) {
                send(con->num, "Not a valid message\n", 30, 0);
                pthread_mutex_unlock(&pm);
                continue;
            }
            char ** tk = tok(buf);
            char* stat = run(tk, con, c->i, bytes);//571
            char prefix[5];
            strncpy(prefix, stat, 4);
            prefix[4] = '\0';
            if(status[c->i]==1&&(strcmp(prefix, "DRAW") == 0 && (strcmp(tk[3], "R") != 0 && strcmp(tk[3], "A") != 0))) {
                send(con->num, "invalid response for draw \n", 36, 0);                 //printf("[%s:%s] got DRAW\n", c->h, c->port);
                freestringarray(tk);
                free(stat);
                pthread_mutex_unlock(&pm);
                continue;
            }
            if(strcmp(prefix, "DRAW") == 0) {
                if(status[c->i] == 1) {
                    if(strcmp(tk[3], "R") ==0) {
                        status[c->i] = 0;
                        send(c->otherplayer->num,stat, strlen(stat), 0);
                    }
                    else if(strcmp(tk[3], "A") == 0) {
                        send(con->num, "OVER DRAW\n", 16, 0);
                        send(c->otherplayer->num, "OVER DRAW\n", 16, 0);
                        freestringarray(tk);
                        free(stat);
                        endgame(c);
                        break;
                    } 
                    else {
                        send(con->num, "Invalid response for draw\n", 36, 0);
                        freestringarray(tk);
                        free(stat);
                        pthread_mutex_unlock(&pm);
                        continue;
                    }
                }
                else if(strcmp(tk[3], "S") == 0) {
                    status[c->i] =1;
                    send(c->other->num, stat, strlen(stat), 0);

                } 
                else {
                    send(con->num, "Invalid response for draw\n", 35, 0);
                    freestringarray(tk);
                        free(stat);
                        pthread_mutex_unlock(&pm);
                        continue;
                }
            }
            else {
                send(con->num, stat, strlen(stat), 0);
            }
            freestringarray(tk);
            if(strcmp(prefix, "INVL") == 0) {                
                free(status);
                pthread_mutex_unlock(&pm);
                continue;
            }
            else if(strcmp(prefix, "MOVD") == 0) {
                send(c->other->num, status, strlen(status) , 0);
                char t[b]; 
                if(checkwinner(c->i) == c->num) {
                    char tmp[b];
                    strcpy(t, "over|");
                    strcpy(tmp, "over|");
                    char name[200];
                    strcpy(name, con->name);
                    int sz = strlen(name) + 11;
                    char b[10];
                    sprintf(b, "&d", sz);
                    strcat(t, b);
                    strcat(tmp, b);
                    strcat(t, "|W|");
                    strcat(tmp, "|L|");
                    strcat(t, name);
                    strcat(tmp, name);
                    strcat(t, " won| \n");
                    strcat(tmp, " won| \n");
                    send(con->num, t, strlen(t), 0);
                    send(c->otherplayer->num, tmp, strlen(tmp), 0);
                    free(status);
                    endgame(c);
                    break;
                }
                else if(checkwinner(c->i) == 2) { 
                    strcpy(t, "Over  7 d draw");
                    send(con->num, t, strlen(t), 0);
                    send(c->otherplayer->num,t, strlen(t), 0);
                    free(status);
                    endgame(c);
                    break;
                }

            }
            else if(strcmp(prefix, "OVER") == 0) {
                char t[300];
                strcpy(t, status);
                t[8]='W';
                send(c->otherplayer->num, t, strlen(status), 0);
                free(status);
                endgame(c);
                break;
            }
            turn[c->i] = (turn[c->i] + 1) %2;
            free(status);
            pthread_mutex_unlock(&pm);
            pthread_cond_broadcast(&prc);

        } else {
            while(turn[c->i] != c->num) {
                pthread_cond_wait(&prc, &pm);
            }
        }
    }
        playerremove(c->player->name);
        removeBoard(c->i);
        close(c->player->num);
        free(c->player);
        free(c);
        pthread_mutex_unlock(&pm);
        return NULL;
}

void *readdata(void *a) {//689
    struct connection *c = arg;
    char buf[b], h[hhh], port[p];
    int byte;
    int error;
    int started = FALSE;
    error = getnameinfo((struct sockaddr *)&c->address, c->address_length, h, hhh, port, p,NI_NUMERICSERV );
    if(error) {
        fprintf(stderr, "info : %s\n",gai_strerror(error) );
        strcpy(hhh, "??");
        strcpy(port, "??");
    }
    pritnf("Connection %s:%s\n", hhh, port );
    send(c->num, "Can read\n", 10, 0);
    while(a && (byte = read(c->num, buf, b)) > 0) { //706
        pthread_testcancel();
        buf[byte-1] == '\0';
        pritnf("[%s:%s] read %d bytes {%s}\n", hhh, port, byte, buf);
        if((byte < 8) || (buf[0] == '|') || (buf[byte-2] != '|')) {
            printf("[%c][%c]\n", buf[0], buf[byte-2]);
            send(c->num, "not valid message\n", 30, 0);
            send(c->num, "Can read\n", 10, 0);
            continue;
        }
        char ** tk = parasedtok(buf);
        const char* status = start(tk, c, byte);
        send(c->num, status, strlen(status), 0);

        char prefix[5];
        strncpy(prefix, status, 4);
        prefix[4] = '\0';
        if(strcmp(prefix, "INVL") == 0) {
            send(c->num, "Can read\n", 10, 0);
            freestringarray(tk);
            continue;
        }
        pthread_mutex_lock(&pm);
        if(ready == 2) {
            p2 = c;
            current = freeindex();
            board[current] = startBoard();
            pthread_cond_broadcast(&prc);
            pthread_t tii;

            struct game *wc = malloc(sizeof(Struct game)); //737
            wc -> player = c;
            wc -> i = current;
            wc-> num = 1;
            wc-> otherplayer = p1;
            strcpy(wc->hhh, hhh);
            strcpy(wc->port, port);
            pthread_create(&tii, worker, wc );
            pthread_cancel(c->tii);
            c->tii = tii;
            pthread_detach(tii);
            started = TRUE;
            freestringarray(tk);
            pthread_mutex_unlock(&pm);
            break;
        }
        else {
            p1 = c;
            while(ready < 2) {
                pthread_cond_wait(&prc, &pm);
            }
            pthread_t fd;
            struct game *wc = malloc(sizeof(struct game));
            wc -> player = c;
            wc -> i = current;
            wc-> num = 0;
            wc-> otherplayer = p2;
            strcpy(wc->hhh, hhh);
            strcpy(wc->port, port);
            turn[current] = 0;
            status[current] = 0;
            ready = 0;
            pthread_create(&tii, worker, wc );
            pthread_cancel(c->tii);
            c->tii = tii;
            pthread_detach(tii);
            started = TRUE;
            freestringarray(tk);
            pthread_mutex_unlock(&pm);
            break;
        }
        freestringarray(tk);
        send(c->num, "Can read\n", 10, 0);

    }
    if(byte == 0) {
        printf("[%s:%s] got EOF ", hhh, port);
    } else if(byte == -1) {
        printf("[%s:%s] ending: %s\n", hhh, port, strerror(errno));

    }
    else {
        printf("[%s:%s] ending: %s\n", hhh, port, strerror(errno));
    }

    if(!started) {
        close(c->num);
        free(c);
    }
    return NULL;
}

int main(int a, char **argv) {
    sigset_t mask;
    struct connection *c;
    int err;
    pthread_t t;
    char *s = argc == 2 ? argv[1] : "15000";
    printf("[%s]\n", argv[2]);
    installh(&mask);
    int lis = openlistn(s, size);
    if(lis < 0) exit(EXIT_FALIURE);
    printf("Listen for incoming connection on %s\n", s);
    while (a) {
        pthread_testcancel();
        c = (struct connection *)malloc(sizeof(struct connection));
        c-> address_length = sizeof(struct sockaddr_storage);
        c->num = accept(lis, (struct sockaddr *)&c->address_length);
        if(c->num <0) {
            perror("accept");
            free(c);
            continue;
        }

        err = pthread_sigmask(SIG_BLOCK, &mask, NULL);
        if(err != 0) {
            fprintf(stderr, "sigmaks: %s", strerror(err));
            exit(EXIT_FALIURE);
        }
        err = pthread_create(&t, NULL, readdata, c);
        c->t = t;
        if(err != 0){
            fprintf(stderr, "pthread create: %s\n", strerror(err));
            close(c->num);
            free(c);
            continue;
        }

        pthread_detach(t);
        err = pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
        if(err != 0) {
            fprintf(stderr, "sigmaks: %s", strerror(err));
            exit(EXIT_FALIURE);
        }
    }
    freeplayer();
    puts('Shut down');
    close(lis);
    return EXIT_SUCCESS;

}