#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "util.h"

#define SERVER_STRING "Server: zerver/0.1\r\n"

void fatal(const char*);
int setup_listen_socket(int port);
void server_loop(int server_socket);
void handle_client(int client_socket);
void handle_http_method(char* method_buffer, int client_socket);
void handle_get_method(char* path, int client_socket);
void handle_post_method(char* path, int client_socket);
void handle_unimplemented_method(int client_socket);
void handle_http_404(int client_socket);
void send_headers(const char* path, off_t len, int client_socket);
void transfer_file_contents(char* file_path, int client_socket, off_t file_size);

const int DEFAULT_SERVER_PORT = 5051;

void fatal(const char* message){
   fprintf(stderr, "%s", message);
   perror(message);
   exit(1);
}

int setup_listen_socket(int port) {
   int sock;
   struct sockaddr_in server_addr;

   // create TCP socket
   // socket(AF_INET, socket_type, protocol);
   sock = socket(PF_INET, SOCK_STREAM, 0);
   if (sock == -1)
      fatal("socket()");

   int enable = 1;
   // SO_REUSEADDR allow OS to reuse the port (like after restart)
   if (setsockopt(sock,
                  SOL_SOCKET, SO_REUSEADDR,
                  &enable, sizeof(int)) < 0)
      fatal("setsockopt(SO_REUSEADDR)");

   // zero a bytestring (used to be bzero, using memset instead)
   memset(&server_addr, 0, sizeof(server_addr));
   // define socket address and port
   server_addr.sin_family = AF_INET;
   // htons converts from host byte order to network byte order (short)
   // basically LSB to MSB
   server_addr.sin_port = htons(port);
   // bind to all available network interfaces (hton long)
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

   // bind the identity (addr and port) to a socket
   // gives socket an address and allows client to reach it
   // bind() and listen() system calls turn program into network server

   if (bind(sock,
            (const struct sockaddr*)&server_addr,
            sizeof(server_addr)) < 0)
      fatal("bind()");

   // queue limit on listen -- can edit /proc/sys/net/core/somaxconn
   // default to 128 on Linux
   if (listen(sock, 10) < 0)
      fatal("listen()");

   return sock;
}

void server_loop(int server_socket) {
   struct sockaddr_in client_addr;
   socklen_t client_addr_len = sizeof(client_addr);
   while (1) {
      int client_socket = accept(server_socket,
                                 (struct sockaddr*)&client_addr,
                                 &client_addr_len);
      if (client_socket == -1)
         fatal("accept()");

      handle_client(client_socket);
      close(client_socket);
   }
}

void handle_client(int client_socket) {
   char line_buffer[1024];
   char method_buffer[1024];
   int method_line = 0;

   while (1) {
      // get_line is slow, in real web server would likely read request in chunks
      get_line(client_socket, line_buffer, sizeof(line_buffer));
      method_line++;

      unsigned long len = strlen(line_buffer);

      // first line has the HTTP method/verb -- other lines discarded
      if (method_line == 1) {
         if (len == 0)
            return;
         // change
         strcpy(method_buffer, line_buffer);
      } else {
         if (len == 0)
            break;
      }
   }
   handle_http_method(method_buffer, client_socket);
}

void handle_http_method(char* method_buffer, int client_socket){
   char* method;
   char* path;

   method = strtok(method_buffer, " ");
   strtolower(method);
   path = strtok(NULL, " ");

   if (strcmp(method, "get") == 0)
      handle_get_method(path, client_socket);
   else if (strcmp(method, "post") == 0)
      handle_post_method(path, client_socket);
   else
      handle_unimplemented_method(client_socket);
}


void handle_http_404(int client_socket) {
   return;
}

void handle_get_method(char* path, int client_socket) {
   char final_path[1024];

   //if (handle_app_get_routes(path, client_socket) == METHOD_HANDLED)
   //   return;

   // if path has trailing slash --> return index in that dir
   if (path[strlen(path) - 1] == '/') {
      strcpy(final_path, "public");
      strcat(final_path, path);
      strcat(final_path, "index.html");
   } else {
      strcpy(final_path, "public");
      strcat(final_path, path);
   }

   // use stat() system call to get info about file on disk
   // like if its a dir or file etc.
   struct stat path_stat;
   if (stat(final_path, &path_stat) == -1) {
      fprintf(stderr, "404 Not Found: %s\n", final_path);
      printf("404 Not Found: %s\n", final_path);
      handle_http_404(client_socket);
   } else {
      // found the file
      if (S_ISREG(path_stat.st_mode)) {
         send_headers(final_path, path_stat.st_size, client_socket);
         transfer_file_contents(final_path, client_socket, path_stat.st_size);
         printf("200 %s %ld bytes\n", final_path, path_stat.st_size);
      } else {
         fprintf(stderr, "404 Not Found: %s\n", final_path);
         printf("404 Not Found: %s\n", final_path);
         handle_http_404(client_socket);
      }
   }
}


void handle_post_method(char* path, int client_socket) {
   return;
}

void handle_unimplemented_method(int client_socket) {
   return;
}

void send_headers(const char* path, off_t len, int client_socket) {
   char small_case_path[1024];
   char send_buffer[1024];
   strcpy(small_case_path, path);
   strtolower(small_case_path);

   strcpy(send_buffer, "HTTP/1.0 200 OK\r\n");
   send(client_socket, send_buffer, strlen(send_buffer), 0);
   strcpy(send_buffer, SERVER_STRING);
   send(client_socket, send_buffer, strlen(send_buffer), 0);

   const char* file_ext = get_filename_ext(small_case_path);
   if (strcmp("html", file_ext) == 0)
      strcpy(send_buffer, "Content-Type: text/html\r\n");
   send(client_socket, send_buffer, strlen(send_buffer), 0);

   // send the content-length header --> just file size
   sprintf(send_buffer, "content-length: %ld\r\n", len);
   send(client_socket, send_buffer, strlen(send_buffer), 0);

   // send "\r\n" on own line to signal end of header
   strcpy(send_buffer, "\r\n");
   send(client_socket, send_buffer, strlen(send_buffer), 0);

   // ends the processing of the client request here
}

void transfer_file_contents(char* file_path, int client_socket, off_t file_size) {
   int fd;

   fd = open(file_path, O_RDONLY);
   sendfile(client_socket, fd, NULL, file_size);
   close(fd);
}

int main(int argc, char** argv) {
   int server_port = DEFAULT_SERVER_PORT;

   int server_socket = setup_listen_socket(server_port);

   printf("Zerver running on port %d\n", server_port);
   //signal(SIGINT, print_stats);
   server_loop(server_socket);
}
