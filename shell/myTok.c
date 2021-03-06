#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "myTok.h"
#include "helpers.h"
#include <errno.h>

char** path;
char** envparams;

int launcher(int argc, char *argv[], char* path[], char *envp[]){

  int rc = 0;
  //we first check if the program starts with '/' this will be the case for explicit filenames.
  if(*argv[0] == '/'){
	rc = fork();
  	if (rc < 0) { // fork failed, exit
    		fprintf(stderr, "Fork failed\n");
    		exit(1);
    	} else 
    	if (rc == 0) { // child 
     		execve(argv[0], argv, envp); 
      		fprintf(stderr, "Command not found\n\n");
     		exit(1);
    	} 
    	else { // parent 
      		int wc = wait(NULL);
    	}
    	return 0;
  }else{//if executable name is used instead.
  	rc = fork();
  	char **currPath = path;
	if (rc < 0) { // fork failed, exit
    		fprintf(stderr, "Fork failed\n");
    		exit(1);
    	} else 
    	if (rc == 0) { // child
		while(*currPath != 0){
		     char *newName = prepName(*currPath, argv[0]);//we will construct the full path name to try it
		     //write(1, "w3\n", 4);//for debugging
     		     execve(newName, argv, envp);
		     free(newName);//free useless construct.
		     currPath += 1;
		}
		fprintf(stderr, "%s\n", strerror(errno));//print the error
     		exit(1);
    	} 
    	else { // parent 
      		int wc = wait(NULL);
    	}  
  }
}


//this method will take a string and break it into tokens, then return a pointer to the array containing the tokens
//precondition, input is of the type "h e l l o 0"
char **myTok(char *str, char delim){

//STEP 1: count number of tokens in string and allocate memory for them in char* array.
  int numTokens = 0; //this number will be used to allocate the size of the array of tokens
  char *iter = str;  //to iterate through the string
  int lastChar = 0; //these two markers will tell us whether we are entering, inside or exiting a token, effectively boolean.
  int current = 0;
 
  while(*iter != 0){

    //if something other than the delimiter is found change current.
    if(*iter == delim){
      current = 0;
    }else{
      current = 1;
    }

    //if current is 1 and lastChar is 0, we encountered the start of a token
    if((lastChar == 0)&&(current == 1)){
      numTokens++;
    }
    //advance comparators and the iterator
    lastChar = current;
    iter++;
    
  }

  //we add space for null terminator.
  numTokens++;
  
  //create the array of char pointers.
  char **tokens = (char**) calloc(numTokens, sizeof(char*));
  
//STEP 2: measure the size of each token and allocate memory in a char array
  iter = str; //reset
  lastChar = 0; //reset
  current = 0; //reset
  int endOfInput = 0; //effectively boolean to know when we reach end of input and when we find a token
  int i = 0; //to index into the tokens array
  int sizeCurrent = 0; //to know size of current token
  
  while(!endOfInput){
    
    //check current character being read.
    if((*iter == delim)||(*iter == 0)/*||(*iter == '\n')*/){
      current = 0;

      //if we have reached the end of the string of characters
      if((*iter == 0)/*||(*iter == '\n')*/){
	endOfInput = 1;
      }
      
    }else{
      current = 1;
    }

    //if in token, increase size count.
    if(current == 1){
      sizeCurrent++;
    }
    
    //if we've reached the end of a token
    if((lastChar == 1)&&(current == 0)){
      
      sizeCurrent++; //making space for null terminator
      *(tokens + i) = (char*)calloc(sizeCurrent, sizeof(char));     
      sizeCurrent = 0; //reset
      i += 1; //advance indexer for tokens array.
    }
	  
    //advance
    lastChar = current;
    iter++;
  }

//STEP 3: copy the tokens into the arrays of chars.
  iter = str; //reset
  i = 0;
  int j = 0; //this will be to index into the array of chars.
  lastChar = 0;
  current = 0;
  endOfInput = 0;
  
  while(!endOfInput){
    
    if((*iter == delim)||(*iter == 0)/*||(*iter == '\n')*/){
      current = 0;
      if((*iter == 0)/*||(*iter == '\n')*/){
	endOfInput = 1;
      }
    }else{
      current = 1;
    }

    //if a character is found it is immediately added into the array
    if(current == 1){
      *(*(tokens + i) + j) = *iter;
      j++;
    }
    
    //everytime we exit a token we advance and reset the indexers
    if((lastChar == 1)&&(current == 0)){    
      i += 1;
      j = 0;
    }
    lastChar = current;
    iter++;
  }
  return tokens;
}

//this method helps the shell support only simple pipes of the type a|b
void analyzer(char* origString, char** pathVector, char** envp){

  path = pathVector;
  envparams = envp;

  //first we need to find number of '&' in original.
  int andCount = ocurrencesOf('&', origString);
  
  //we must then find if the string ends with an '&'.
  int lastBackAnd = 0;//zero means no '&' in last pos, 1 means yes.
  if(origString[tokenLen(origString) - 1] == '&'){
    lastBackAnd = 1;
  }
  //now we must create the array to hold the bg processes.
  char** processes = myTok(origString, '&');
  int numProcs = countTokens(processes);
  int i = 0;
  
  while(i < numProcs){

    if(((i + 1) == numProcs)&&(!lastBackAnd)){//the last process is not a background process
      runForeGround(i + 1, processes[i]);
    }else{//everything is a bg
      runBackGround(i + 1, processes[i]);
    }
    i++;
  }
  //we must free the 2d array "processes" here.
  
}

void prepGusintah(char *process){

  //to remove spaces and nls at end.
  char* gusintah = rmTailSpaces(process);
  
  //tokenize by '>'
  char** gusToks = myTok(process, '>');

  //use only the first argument into the second, more than two would generate empty files.
  if(countTokens(gusToks) > 1){

    char** dispToks = myTok(gusToks[1], ' ');//use the first word found on the right side of the gusintah as file name.
    int saved_stdout = dup(1);
    close(1);//close stdout
    open(dispToks[0], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);//open the file outputstream

    freeArray(dispToks);//array no longer useful
    runWithPipes(gusToks[0]);//run the left side of expression
    close(1);
    dup(saved_stdout);//return things back to normal once done
    
  }else{
    //run normally, there is no file to write into.
    runWithPipes(process);
  }
  
  freeArray(gusToks);

}

void runForeGround(int num, char* process){
  prepGusintah(process);
}


void runWithPipes(char* process){

  //clean the end of the string
  char * trimmedPip = rmTailSpaces(process);

  //tokenize by pipes
  char** pipingToks = myTok(trimmedPip, '|');
  free(trimmedPip);
  
  int count = countTokens(pipingToks);

  char** command;
  char** currPath;
  
  if(count > 1){//only do the piping if there is at least one pipe symbol.
  int stat;
  int i = 0;
  pid_t pid;
  
  //we use a large pipe and will have each spot of it be allocaed for a child
  int pipes[2*count];
  for(i = 0; i<count; i++){ 
    if(pipe(pipes + i*2) < 0){//pipe at avery 2n index
      perror("couldn't pipe\n");
      exit(EXIT_FAILURE);
    }
  }

  int j = 0;
  while(*pipingToks != 0){//iterate through the tokenized by pipes array
    pid = fork();//we only fork once
    if(pid == 0){//child

      if(*(pipingToks + 1) != 0){//check if this is not the last pipe

	//dup the stdout into the next adjacent index
	if(dup2(pipes[j + 1], 1)<0){
	  perror("dup2\n");
	  exit(EXIT_FAILURE);
	}
      }
      //if this is the first index don't try dup for stdin
      if(j != 0){
	if(dup2(pipes[j-2], 0) < 0){//since j will increment in 2's
	  perror("dup2\n");
	  exit(EXIT_FAILURE);
	}
      }
      //we close the original fds
      for(i = 0; i < (2*count); i++){
	close(pipes[i]);
      }

      command = myTok(*pipingToks, ' ');//this is the final product of command and params.
      currPath = path;
      while(*currPath != 0){
	    char *newName = prepName(*currPath, command[0]);//we will construct the full path name to try it
	    //write(1, "w3\n", 4);//for debugging
      	    execve(newName, command, envparams);
	    free(newName);//free useless construct.
	    currPath += 1;
      }
      //print the error, this stmnt only reached if none worked
      fprintf(stderr, "%s\n", strerror(errno));
      exit(1);
    }else
    if(pid < 0){//error in forking
	perror("error");
	exit(EXIT_FAILURE);
    }
    pipingToks++;
    j += 2;
  }
  //we close the fds
  for(i = 0; i < 2*count; i++){
    close(pipes[i]);
  }
  //wait for each children to be done
  for(i = 0; i<count + 1; i++){
    wait(&stat);
  }
  }else{//if there is only one command with no pipes
    command = myTok(*pipingToks, ' ');
    launcher(0, command, path, envparams);
  }
}

void runBackGround(int num, char* process){

  pid_t pid = fork();//fork the process
  int r;
  if(pid < 0){
    fprintf(stderr, "fork failed for background\n");
    exit(1);
  }
  if(pid == 0){//child
    
    r = setpgid(0,0);//set self in different process group, essentially in background.

    prepGusintah(process);
    
    exit(1);// we must exit the child once so prompt isnt printed twice.
    
  }else{//parent
    //set child in different process group, essentially in background.
    r = setpgid(pid,0);
    //would be nice to print bg process' id.
    fprintf(stderr,"[%d] %d\n", num, pid);
    //this branch is the normal shell.
  }
  
}
