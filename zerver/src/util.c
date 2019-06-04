#include "util.h"

void fatal(const char* message){
   fprintf(stderr, "%s", message);
   perror(message);
   exit(1);
}

void strtolower(char *str) {
    for (; *str; ++str)
        *str = (char)tolower(*str);
}

void print_stats() {
    double          user, sys;
    struct rusage   myusage, childusage;

    if (getrusage(RUSAGE_SELF, &myusage) < 0)
        fatal("getrusage()");
    if (getrusage(RUSAGE_CHILDREN, &childusage) < 0)
        fatal("getrusage()");

    user = (double) myusage.ru_utime.tv_sec +
                    myusage.ru_utime.tv_usec/1000000.0;
    user += (double) childusage.ru_utime.tv_sec +
                     childusage.ru_utime.tv_usec/1000000.0;
    sys = (double) myusage.ru_stime.tv_sec +
                   myusage.ru_stime.tv_usec/1000000.0;
    sys += (double) childusage.ru_stime.tv_sec +
                    childusage.ru_stime.tv_usec/1000000.0;

    printf("\nuser time = %g, sys time = %g\n", user, sys);
    exit(0);
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
