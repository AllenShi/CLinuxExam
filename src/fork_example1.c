#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUF_SIZE 1*1024
#define MAX_COUNT 200

void main() {
  pid_t pid;
  int i;
  char buf[BUF_SIZE];

  fork();
  pid = getpid();
  for(i = 1; i <= MAX_COUNT; i++) {
    sprintf(buf, "This line is from pid %d, value = %d\n", pid, i);
    write(1, buf, strlen(buf));
    /*buffered io might cause potencial issue*/
    // printf("This line is from pid %d, value = %d\n", pid, i);
  }
}
