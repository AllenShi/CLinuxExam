/*
 * =====================================================================================
 *
 *       Filename:  fork_tcp_client.c
 *
 *    Description:  tcp client using fork
 *
 *        Version:  1.0
 *        Created:  02/02/2016 23:23:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Allen Shi (), jinliangshi@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

#define MAX_SIZE 4*1024

int main() {

    struct sockaddr_in myaddr ,serveraddr;
    int sock_fd;
    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    memset(&myaddr,'0',sizeof(myaddr));
    myaddr.sin_family=AF_INET;
    myaddr.sin_port=htons(8888);
    myaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(sock_fd==-1) {
        perror("socket");
    }

    int len=sizeof(myaddr);
    if(connect(sock_fd,(const struct sockaddr*)&myaddr,len)==-1) {
        perror("connect");
    }

    fprintf(stdout,"Client Online....");
    char s[MAX_SIZE];
    
    //gets(s);
    //send(sock_fd,s,MAX_SIZE,0);
    recv(sock_fd,&s,MAX_SIZE,0);
    fprintf(stdout,"Server says....");
    puts(s);
    recv(sock_fd,&s,MAX_SIZE,0);
    fprintf(stdout,"Server says....");
    puts(s);

    sleep(10);  
    close(sock_fd);
    return 0;
}
