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

const char *server_ip;
int server_port;

void * connection_handler(void *thread_id);

int main(int argc, char *argv[]) {

    if(argc < 4) {
        printf("Usage: pthread_tcp_client.bin <server ip> <server port> <max client>\n");
        exit(EXIT_FAILURE);
    }

    server_ip = argv[1];
    server_port = atoi(argv[2]);
    int max_client = atoi(argv[3]);

    pthread_t worker_thread;
    for(long i = 0; i < max_client; i++) {
        if(pthread_create(&worker_thread, NULL, connection_handler, (void *)(i+1))) {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
    }

    pthread_exit(NULL);
    return 0;
}

void * connection_handler(void *thread_id) {
    long t_id = (long)thread_id;
    int sock_fd;

    struct sockaddr_in server_addr;
    char send_buffer[MAX_SIZE], recv_buffer[MAX_SIZE];

    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("creating socket failed");
        exit(EXIT_FAILURE);
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);
    
    if(connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connecting to server failed");
        exit(EXIT_FAILURE);
    }

    printf("connection is established :%ld\n", t_id);
    while(1) {
        printf("Thread id: %ld\n", t_id);
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
