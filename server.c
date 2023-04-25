#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"conio.h"
#include<time.h>

int num;
int portNumber;
struct sockaddr_in server; 
struct sockaddr_in client;
struct sockaddr_in server_address; 
int number;
unsigned int client_length;
int postion[2]
int winning = 0;
char pos[9];

int winner(char arr[], char character) {
    int z, y, x;
    if(arr[0] == '0' && arr[1] == character && arr[2] == character)
    {
        return 1;
    }
    else if(arr[3] == x && arr[4] == x && arr[5] == x) {
        return 1;
    }
    else if(arr[6] == x && arr[7] == x && arr[8] == x) {
        return 1;
    } 
    else if(arr[0] == x && arr[3] == x && arr[6] == x) {
        return 1;
    } 
    else if(arr[1] == x && arr[4] == x && arr[7] == x) {
        return 1;
    }
    else if(arr[2] == x && arr[5] == x && arr[8] == x) {
        return 1;
    }
    else if(arr[0] == x && arr[4] == x && arr[8] == x) {
        return 1;
    } 
    else {
        return 0;
    }
}

void create() {
    if((num = socket(AF_INET, SOCK_STREAM,0)) < 0) 
    {
        perror("socket");
        exit(1);
    }
}
void bindserver() 
{
    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(portNumber);

    server.sin_addr.s_addr = INADDR_ANY;
    if(bind(num,(struct sockaddr*) &server, sizeof(server)) < 0) 
    {
        perror("Bind has failed");
        exit(1);
    }
}
void listenserver() 
{
    if(listen(num, 5) < 0) {
        perror("listen");
        exit(1);
    }
}
void acceptserver() 
{
    client_length = sizeof(client);
    number = accept(num,(struct sockaddr*) &client, &client_length);
    if(number < 0) {
        perror("accept");
        exit(1);
    }
    else 
    {
        printf("Your friend is joined: \n");
    }
}

void clientconnect() {
    bzero((char *)&server, sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port = htons(portNumber);
    if(connect(num, (struct sockaddr*) &server,sizeof(server)) <0) 
    {
        perror("CONNECT");
        exit(1);
    }
    else 
    {
        printf("You are connected");
    }
}

void readwrite(int n) {
    char cc[0];
    int i, j;
    if(portNumber%2) {
        i = 4;
        j = 5;
    } else {
        i = 5;
        j = 4;
    }
    if(n == 1) {
        for (int i = 0; i < 4; i ++) {
            recv(number, &postion, 2*sizeof(int), 0);
            recv(number, &winner, sizeof(int), 0);
            pos[postion[0] - 1] = '1';
            printf(" %c | %c | %c\n", pos[0], pos[1], pos[2]);
            printf(" %c | %c | %c\n", pos[3], pos[4], pos[5]);
            printf(" %c | %c | %c\n", pos[6], pos[7], pos[8]);
            printf("Next person's turn: %d\n", postion[0]);

            if(winning == 1) {
                printf("The guest win is ");
                printf("");
                break;
            }
            do {
                printf("Your turn");
                scanf("%d", &postion[0]);
                system("clear");
                if(pos[postion[0] - 1] != ' ')
                {
                    printf("wrong choice");
                    pritnf("");
                }
            }while(pos[postion[0]-1]!=' ');
            system("clear");
            pos[postion[0] - 1] = '0';
            cc[postion[0]-1] = '0';
            winning = winner(cc, '0');
            send(number, &pos, 2*sizeof(int), 0);
            send(number, &winning, sizeof(int), 0);
            printf(" %c|%c|%c\n", pos[0], pos[1], pos[2]);
            printf(" %c | %c | %c\n", pos[3], pos[4], pos[5]);
            printf(" %c | %c | %c\n", pos[6], pos[7], pos[8]);
            if(winning ) {
                printf("Yyayyayya You won\n");
                break;
            }
        }
        close(number);
    }
    else if(n == 2) {
        for(int i = 0; i < 5; i ++) {
            do{
                printf("You go now: ");
                scanf("%d", &postion[0]);
                if(pos[postion[0]-1] != ' ')
                {
                    printf("Wrong one chosen\n");
                    printf("try again\n")
                }
            }while(pos[postion[0] - 1] != ' ');
            pos[postion[0]-1] = '1';
            cc[postion[0]-1] = '1';
            winning = winner(cc, '1');
            send(num , &postion, 2*sizeof(int), 0);
            send(num, &winning, sizeof(int), 0);
            printf(" %c | %c | %c\n", pos[0], pos[1], pos[2]);
            printf(" %c | %c | %c\n", pos[3], pos[4], pos[5]);
            printf(" %c | %c | %c\n", pos[6], pos[7], pos[8]);
            if(winning) {
                printf("YAYAYAYYA YOU WON");
                break;
            }
            system('clear');
            recv(num, &postion, 2*sizeof(int), 0);
            recv(num, &winning, sizeof(int),0);
            pos[postion[0]-1] = '0';
            printf(" %c | %c | %c \n", pos[0], pos[1], pos[2]);
            printf(" %c | %c | %c\n", pos[3], pos[4], pos[5]);
            printf(" %c | %c | %c\n", pos[6], pos[7], pos[8]);
            printf("");
            printf("Guest's turn: %d\n", postion[0]);
            if(winning) {
                printf("Guest won so they got: ")
                break;
            }

        }
        close(num)
    }

    int main() {
        system("clear");
        printf("Press anything: ");
        getch()
        system("clear");
        printf("Create room press 1\n");
                printf("Join room press 2\n");
        printf("Offline room press 3\n");
        printf("Enter your decission: ");
        int pick;
        int code;
        scanf("%d", &pick);
        system("clear");
        char *args[] = {"./simple", NULL};
        for(int i = 1; i < 10; i++) {
            pos[i]=  ' ';
        }
        switch(pick){
            case 1: 
                srand(time(0));
                code = (rand()%(999 - 5000 + 1)) + 5000;
                printf("Code: %d ", code);
                printf("Enter same code for player 2: ");
                portNumber = code;
                printf("Wait for the other player \n");
                create();
                bindserver();
                listenserver();
                acceptserver();
                printf("1 | 2 | 3 ");
                printf("\n");
                printf("\n4 | 5 | 6");
                printf("\n");
                printf("\n7 | 8 | 9");
                printf("\n");
                readwrite(pick);
            case 2:
                printf("Code: ");
                scanf("%d", &code);
                create();
                printf("Wait for the other player \n");
                portNumber = code;
                clientconnect();
                printf("1 | 2 | 3 ");
                printf("\n");
                printf("\n4 | 5 | 6");
                printf("\n");
                printf("\n7 | 8 | 9");
                printf("\n");        
                readwrite(pick);
            case 3;
               execv(args[0], args);

        }


        
    }
}