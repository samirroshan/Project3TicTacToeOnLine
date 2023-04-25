#include<stdio.h>
char p[9];
int winning = 0;
int win(char character) {
    if(p[0] == character && p[1] == character && p[2] == character) {
        return 1;
    } else if(p[3] == character && p[4] == character && p[5] == character) {
        return 1;
    } else if (p[6] == character && p[7] == character && p[8] == character) {
        return 1;
    } else if (p[0]== character && p[3] == character && p[6] == character) {
        return 1;
    }else if (p[1] == character && p[4] == character && p[7] == character) {
        return 1;
    } else if (p[2] == character && p[5] == character && p[8] == character) {
        return 1;
    } else if(p[0] == character && p[4] == character && p[8] == character) {
        return 1;
    } else {
        return 0;
    }


}

int main() {
    char first_name[10], server_name[10];
    printf("First Player: ");
    scanf("%s", first_name);
    for(int j = 0; j < 9; j++) {
        p[j] = ' ';
    }
    printf("Player 2: " );
    scanf("%s", server_name);

    for(int j= 1; j < 10; j++) {
        printf(" %c | %c | %c \n",p[0],p[1], p[2]);
        printf(" %c | %c | %c \n",p[3],p[4], p[5]);
        printf(" %c | %c | %c \n",p[6],p[7], p[8]);

        int ps;
        if(j % 2 != 0) 
        {
            printf("%s 0 and %s 1\n", first_name, server_name);
            printf("%s turn :", first_name);
            do 
            {
                scanf("%d", &ps);
                ps = ps - 1;
                if(p[ps] != ' ') {
                    printf("Invalid one chosesn ");
                }
            } while(p[ps] != ' ');
            p[ps] = '0';
            winning = win('0');
            if(winning)
            {
                printf("Win %s", first_name);
                break;
            } if(j==9) {
                printf("Game Ended");
            }
        }
        else {
            printf("%s 0 and  %s 1 \n", first_name, server_name);
            printf("%s turn: ", server_name);
            do {
                scanf("%d", &ps);
                ps = ps - 1;
                if(p[ps]!=' ') {
                    printf("Invalid one chossen ");
                }
            } while (p[ps] != ' ');
            p[ps] = '1';
            winning = win('1');
            if(winning) {
                printf("win %s", server_name);
                break;
            }
        }
    }
}