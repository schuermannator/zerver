#ifndef __ZERVER_H_
#define __ZERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include "util.h"

#define SERVER_STRING "Server: zerver/0.1.0\r\n"

void fatal(const char*);
int setup_listen_socket(int port);
//void server_loop(int server_socket);
void* server_loop(void* target);
void handle_client(int client_socket);
void handle_http_method(char* method_buffer, int client_socket);
void handle_get_method(char* path, int client_socket);
void handle_post_method(char* path, int client_socket);
void handle_unimplemented_method(int client_socket);
void handle_http_404(int client_socket);
void send_headers(const char* path, off_t len, int client_socket);
void transfer_file_contents(char* file_path, int client_socket, off_t file_size);

const int DEFAULT_SERVER_PORT = 5051;
const char* http_404_content = \
        "HTTP/1.0 404 Not Found\r\n"
        "Content-type: text/html\r\n"
        "\r\n"
        "<html>"
        "<head>"
        "<title>Zerver: Not Found</title>"
        "</head>"
        "<body>"
        "<h1>Not Found (404)</h1>"
        "<p>Client request for an object that was not found on this server.</p>"
        "</body>"
        "</html>";

#endif // __ZERVER_H_
