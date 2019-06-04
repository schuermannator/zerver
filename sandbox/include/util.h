#ifndef __UTIL_H_
#define __UTIL_H_

int get_line(int sock, char* buf, int size);
void strtolower(char *str);
const char *get_filename_ext(const char *filename);

#endif // __UTIL_H_
