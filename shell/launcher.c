#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int launcher(int argc, char *argv[], char *envp[]){
  int rc = fork();
  if (rc < 0) { // fork failed, exit
    fprintf(stderr, "fork failed\n");
    exit(1);
    } else 
    if (rc == 0) { // child (new process)
      char *myargs[3];
      myargs[0] = strdup("wc"); // program: "wc" (word count)
      myargs[1] = strdup("p3.c"); // argument: file to count
      myargs[2] = NULL; // marks end of array
      execvp(myargs[0], myargs); // TODO: must change to execve
      printf("this shouldn’t print out");
    } else { // parent goes down this path (main)
    int wc = wait(NULL);
    //printf("hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc, (int) getpid());
    }
    return 0;
}
