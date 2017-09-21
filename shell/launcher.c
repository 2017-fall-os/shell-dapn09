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
      write(1, "this shouldn’t print out\n", 25);
    } else { // parent goes down this path (main)
         int wc = wait(NULL);
         if(execRet == 0){
            write(1, "hello, I am parent\n", 19);
         }else{
            write(1, "program exited abnormaly\n", 27);
            return 0;
         }
      }
    return 0;
}
