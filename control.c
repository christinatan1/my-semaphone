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

// union semun {
//   int              val;    /* Value for SETVAL */
//   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
//   unsigned short  *array;  /* Array for GETALL, SETALL */
//   struct seminfo  *__buf;  /* Buffer for IPC_INFO*/
// };

int main(int argc, char * argv[]){
  int semd, shmd, fd;
  union semun sem;
  //struct sembuf sb;

  if (argc == 2){

    if (!strcmp(argv[1],"-c")){ //creating
      semd = semget(KEY, 1 , IPC_CREAT | 0644);
      printf("semaphore created \n");
      semctl(semd, 0, SETVAL, sem);
      sem.val = 1;
      shmd = shmget(KEY, 1000 , IPC_CREAT | 0644);
      printf("shared memory created \n");
      fd = open("test.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
      if (fd == -1){
        printf("error: %s\n", strerror(errno));
      }
      close(fd);
      printf("file created\n");
    }

    if (!strcmp(argv[1],"-r")){ // reading
      semd = semget(KEY, 1 , 0);
      printf("trying to get in\n");
      int fd = open("test.txt", O_RDONLY);
      printf("Story so far: \n");
      char buffer[1000];
      read(fd, buffer, 1000);
      printf("%s\n", buffer);
      close(fd);
      semctl(semd, IPC_RMID, 0);
      printf("shared memory removed\n");
      shmctl(shmd, IPC_RMID, 0);
      printf("semaphore removed\n");
      remove("test.txt");
      printf("file removed\n");
    }

    if (!strcmp(argv[1],"-v")){ // viewing
      int fd = open("test.txt", O_RDONLY);
      printf("Story so far: \n");
      char buffer[1000];
      read(fd, buffer, 1000);
      printf("%s\n", buffer);
      close(fd);
    }
}
}
