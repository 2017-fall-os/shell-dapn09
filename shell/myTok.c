#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "myTok.h"


int launcher(int argc, char *argv[], char *envp[]){

  int rc = fork();
  int execRet = 0;
  //we first check if the program starts with '/'
  if(*argv[0] == '/'){
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
  }else{
  
  	
  
  }
}

//this method gets a reference to the envp vector and returns a vector made from the PATH entries.
char ** getPath(char** envp){
    char **pathVec = 0;// this will be the returned array
    char **currStr = envp;
    while(*currStr != 0){//traverse array of strings.
	int i = 0;
	char *c = *currStr;
	while(*c != 0){// to count elements in string, no need to look into shorter strings than 5.
	    i++;
	    c++;
	}
    	if(i > 5)
	   c = *currStr;
	   if(c[0] == 'P')
	     if(c[1] == 'A')
		if(c[2] == 'T')
		   if(c[3] == 'H')
		      if(c[4] == '='){
			 //write(1, "PATH was found!\n", 17);//for debugging purposes only
			 //write(1, c, 40);//for debugging only
		     	 pathVec = (char**) myTok(&c[5] , ':');//we pass the remainder of the string and the delimiter set to ':'
			 return pathVec;
		      }
	currStr += 1;     
    }
    return pathVec;
}

//helper method used to print the 2D array
void print2DArray(char** array){

  char** tokens = array;
  int i = 0;
  int count = 0;
  
  while(*tokens != 0){

    while(*(*tokens + i) != 0){
      count++;
      i++;
    }
    i = 0;
    write(1, *tokens, count);
    write(1, "\n", 1);
    count = 0;
    tokens++;
  }

}

//this method frees the memory used.
void freeArray(char** array){

  char** iter = array;
  while(*iter != 0){
    free(*iter);
    iter++;
  }
  free(array);
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
