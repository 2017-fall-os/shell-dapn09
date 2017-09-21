#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int launcher(int argc, char *argv[], char *envp[]){
  int rc = fork();
  int execRet = 0;
  if (rc < 0) { // fork failed, exit
    write(1, "fork failed\n", 13);
    exit(1);
    } else 
    if (rc == 0) { // child (new process)
      //char *myargs[3];
      //myargs[0] = strdup("wc"); // program: "wc" (word count)
      //myargs[1] = strdup("p3.c"); // argument: file to count
      //myargs[2] = NULL; // marks end of array
      execRet = execve(argv[0], argv, envp); // TODO: must change to execve myargs[0]
      write(1, "Command not found\n\n", 21);
      exit(1);
    } 
    else { // parent goes down this path (main)
      int wc = wait(NULL);
    }
    return 0;
}
