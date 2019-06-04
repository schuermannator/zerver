#include "util.h"

void strtolower(char *str) {
    for (; *str; ++str)
        *str = (char)tolower(*str);
}


/*
 * Reads the client socket character-by-character and once a line is read, the string
 * is NULL-terminated and the function returns the characters read.
 * This is not a very efficient way of doing things since we are making multiple calls
 * to read(), but it keeps things simple.
 * */

int get_line(int sock, char *buf, int size) {
    int i = 0;
    char c = '\0';
    ssize_t n;

    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
            }
            else
            {
                buf[i] = c;
                i++;
            }
        }
        else
            return 0;
    }
    buf[i] = '\0';

    return (i);
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}
