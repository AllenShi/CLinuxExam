/*
 * =====================================================================================
 *
 *       Filename:  pthread_tcp_server.c
 *
 *    Description:  tcp server using pthread
 *
 *        Version:  1.0
 *        Created:  02/02/2016 18:02:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Allen Shi (), jinliangshi@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


void * connection_handler(void *socket_fd); 

int main(int argc, char *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;

    //Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1) {
        perror("could not create server socket");
    }
    puts("socket created");

    //Prepare server sockaddr structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(3000);

    //Bind
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed.");
        abort();
    }
    puts("bind done");

    //Listen
    listen(server_fd, 3);

    //Accept incoming connection
    puts("waitting for incomming ...");

    int sock_len = sizeof(struct sockaddr_in);
    int *client_fd_copy;
    while((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&sock_len)) > -1) {
        puts("connection accepted");

        pthread_t worker_thread;
        client_fd_copy = malloc(1);
        *client_fd_copy = client_fd;

        if(pthread_create(&worker_thread, NULL, connection_handler, (void *)client_fd_copy) < 0) {
            perror("could not create thread");
            abort();
        }
        puts("delegated to handler");
    }

    if(client_fd < 0) {
        perror("accept failed");
        abort();
    }

    return 0;
}

void * connection_handler(void *socket_fd) {
    //Get the socket descriptor
    int sock = *(int *)socket_fd;
    char send_buffer[512], client_message[2*1024];

    int n;
    while((n = recv(sock, client_message, 2*1024, 0) > 0)) {
        send(sock, client_message, n, 0);
    }
    close(sock);

    if(n == 0) {
        puts("client disconnected");
    }
    else {
        perror("recv failed");
    }
    return 0;
}
