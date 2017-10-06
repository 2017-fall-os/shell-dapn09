#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "myTok.h"
#include "helpers.h"
#include <errno.h>


int launcher(int argc, char *argv[], char* path[], char *envp[]){

  int rc = 0;
  //we first check if the program starts with '/' this will be the case for explicit filenames.
  if(*argv[0] == '/'){
	rc = fork();
  	if (rc < 0) { // fork failed, exit
    		write(1, "fork failed\n", 13);
    		exit(1);
    	} else 
    	if (rc == 0) { // child 
     		execve(argv[0], argv, envp); 
      		write(1, "Command not found\n\n", 20);
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
    		write(1, "fork failed\n", 13);
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
    if((*iter == delim)||(*iter == 0)||(*iter == '\n')){
      current = 0;

      //if we have reached the end of the string of characters
      if((*iter == 0)||(*iter == '\n')){
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
    
    if((*iter == delim)||(*iter == 0)||(*iter == '\n')){
      current = 0;
      if((*iter == 0)||(*iter == '\n')){
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

void runForeGround(int num, char* process){

  //tokenize by '|'
  char** statements = myTok(process, '|');
  
  
  
}

void runBackGround(int num, char* process){

  pid_t pid = fork();//fork the process
  int r;
  if(pid < 0){
    write(1, "fork failed\n", 13);
    exit(1);
  }
  if(pid == 0){//child
    
    r = setpgid(0,0);//set self in different process group, essentially in background.

    //launch here
    
    exit(1);// we must exit the child once so prompt isnt printed twice.
    
  }else{//parent

    //set child in different process group, essentially in background.
    r = setpgid(pid,0);
    //would be nice to print bg process' id.
    printf("[%d] %d\n", num, pid);
    //this branch is the normal shell.
  }
  
}
