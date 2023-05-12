#include "utils.h"

void err_and_kill(const char *fmt, ...) {

    int errnum = errno;
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);

    if (errnum != 0) {
        fprintf(stderr, "(errno = %d): %s\n\n", errnum, strerror(errnum));
        fflush(stderr);
    }

    va_end(args);
    exit(EXIT_FAILURE);
}

char* bin2hex(const unsigned char *bin, size_t len) {

    char *hexits = "0123456789ABCDEF";
    char *hex;

    if (bin == NULL || len <= 0) {
        return NULL;
    }

    int hex_len = (len * 3) + 1;

    hex = malloc(hex_len);
    memset(hex, 0, hex_len);

    for (int i = 0; i < len; i++) {
        hex[i * 3] = hexits[bin[i] >> 4];
        hex[(i * 3) + 1] = hexits[bin[i] & 0x0F];
        hex[(i * 3) + 2] = ' ';
    }

    return hex;
}