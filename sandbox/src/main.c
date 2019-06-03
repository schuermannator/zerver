#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

const int DEFAULT_SERVER_PORT = 5051;

void fatal(const char* message){
   fprintf(1, "%s", message);
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


int main(int argc, char** argv) {
   int server_port = DEFAULT_SERVER_PORT;

   int server_socket = setup_listen_socket(server_port);

   printf("Zerver running on port %d\n", server_port);
}
