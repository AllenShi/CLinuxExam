#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_COUNT 200

void child_process();
void parent_process();

void main() {
  pid_t pid;

  pid = fork();
  if(pid == 0) {
    child_process();
  }
  else {
    parent_process();
  }
}

void child_process() {
  for(int i = 1; i <= MAX_COUNT; i++) {
    printf("This line is from child, value = %d\n", i);
  }
  printf("Child process is done\n");
}

void parent_process() {
  for(int i = 1; i <= MAX_COUNT; i++) {
    printf("This line is from parent, value = %d\n", i);
  }
  printf("Parent process is done\n");
}
