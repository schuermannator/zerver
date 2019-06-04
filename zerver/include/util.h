#ifndef __UTIL_H_
#define __UTIL_H_

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>

void fatal(const char* message);
int get_line(int sock, char* buf, int size);
void strtolower(char *str);
const char *get_filename_ext(const char *filename);
void print_stats();

#endif // __UTIL_H_
