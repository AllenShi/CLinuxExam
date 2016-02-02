/*
 * =====================================================================================
 *
 *       Filename:  fork_tcp_server.c
 *
 *    Description:  tcp server using fork
 *
 *        Version:  1.0
 *        Created:  02/02/2016 23:23:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Allen Shi (), jinliangshi@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include<signal.h>
#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>

int main() {

    struct sockaddr_in myaddr ,client_addr;
    int sock_fd;
    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    memset(&myaddr,'0',sizeof(myaddr));
    myaddr.sin_family=AF_INET;
    myaddr.sin_port=htons(8888);
    myaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(sock_fd==-1) {
        perror("socket");
    }

    socklen_t len=sizeof(myaddr);
    if(bind(sock_fd,( struct sockaddr*)&myaddr,len)==-1)
    {
        perror("bind");
    }
    
    if(listen(sock_fd,10)==-1)
    {
        perror("listen");
    }

    int pid,new;
    static int counter=0;
    for(;;) { 
    	new = accept(sock_fd, (struct sockaddr *)&client_addr, &len);

        if ((pid = fork()) == -1) {
            close(new);
            continue;
        }
        else if(pid > 0) {
            close(new);
            counter++;
            printf("here2\n");
            continue;
        }
        else if(pid == 0) {
            char buf[100];

            counter++;
            printf("here 1\n");
            snprintf(buf, sizeof buf, "hi %d", counter);
            send(new, buf, strlen(buf), 0);
            close(new);
            break;
        }
    }
    printf("here3");
    close(sock_fd);
    return 0;
}
