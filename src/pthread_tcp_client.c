/*
 * =====================================================================================
 *
 *       Filename:  pthread_tcp_client.c
 *
 *    Description:  TCP client using pthread for concurrency
 *
 *        Version:  1.0
 *        Created:  02/02/2016 18:57:35
 *       Revision:  none
 *       Compiler:  gcc
 * *         Author:  Allen Shi (), jinliangshi@gmail.com *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE 512
#define MAX_CLIENT 5

const char *server_ip;
int server_port;

void * connection_handler(void *thread_id);

int main(int argc, char *argv[]) {

    if(argc < 3) {
        printf("Usage: pthread_tcp_client.bin <server ip> <server port>\n");
        abort();
    }

    server_ip = argv[1];
    server_port = atoi(argv[2]);

    pthread_t worker_thread;
    for(int i = 0; i < MAX_CLIENT; i++) {
        if(pthread_create(&worker_thread, NULL, connection_handler, (void *)&i)) {
            perror("could not create thread");
            abort();
        }
    }

    pthread_exit(NULL);
    return 0;
}

void * connection_handler(void *thread_id) {
    int t_id = *(int *)thread_id;
    int sock_fd;

    struct sockaddr_in server_addr;
    char send_buffer[MAX_SIZE], recv_buffer[MAX_SIZE];

    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("creating socket failed");
        abort();
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);
    
    if(connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connecting to server failed");
        abort();
    }

    printf("connection is established :%d\n", t_id);
    while(1) {
        printf("Thread id: %d\n", t_id);
        fgets(send_buffer, MAX_SIZE, stdin);
        send(sock_fd, send_buffer, MAX_SIZE, 0);

        if(recv(sock_fd, recv_buffer, MAX_SIZE, 0) == 0)
            printf("Error");
        else 
            fputs(recv_buffer, stdout);

        bzero(recv_buffer, MAX_SIZE);
        sleep(2);
    }
    close(sock_fd);
    return 0;
}
