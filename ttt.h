#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int key(void) {
    struct old, news, str;
    int charnum;
    tcgetattr(STDIN_FILENO, &old);
    news = old;
    news.c_lflag &= ~(ICANON | ECHO)
    tcsettattr(STDIN_FILENO,TCSANOW , &news);
    charnum = getchar();
    tcsettattr(STDIN_FILENO, TCSANOW, &old);
    return charnum;
}

int getkeyp(void) {
    struct old, news str;
    int charnum;
    tcgetattr(STDIN_FILENO, &old);
    news = old;
    news.c_lflag &= ~(ICANON);
    tcsettattr(STDIN_FILENO, TCSANOW, &news);
    charnum = getchar();
    tcsettattr(STDIN_FILENO, TCSANOW, &old);
    return charnum;
}