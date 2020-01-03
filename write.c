#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <fcntl.h>

#define KEY 24601
struct sembuf sb;
int semd, shmd, fd;
char add[1000];

int main(){
  sb.sem_num = 0;
  sb.sem_op = -1;
  printf("trying to get in\n");
  semd = semget(KEY, 1, 0);
  if (semd < 0){
    printf("error: %s\n", strerror(errno));
  }
  shmd = shmget(24601, sizeof(char *), 0);
  fd = open("test.txt", O_WRONLY | O_APPEND);
  char *prev = shmat(shmd, 0, 0);
  printf("Last addition: %s\n", prev);
  printf("Your addition: ");
  fgets(add, 1000, stdin);
  printf("\n");
  write(fd, add, strlen(add));
  close(fd);
  strcpy(prev, add);
  shmdt(prev);
  sb.sem_op = 1;
  semop(semd, &sb, 1);
  return 0;
}
