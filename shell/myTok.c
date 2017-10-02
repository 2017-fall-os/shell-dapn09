#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "myTok.h"
#include <errno.h>

int tokenLen(char *tok){// tool method for measuring size of tokens
    int i = 0;
    while(*tok != 0){
	i++;
	tok +=1;
	//write(1, "w6\n", 4);//for debugging
    }
    return i;
}

char *prepName(char* currPath, char* argv0){

     int curPathLength = tokenLen(currPath);
     int argLength = tokenLen(argv0);
     char *newName = (char*)calloc((curPathLength + 1 + argLength + 1), 1);
     int i = 0;//the index to newName
     int j = 0;
     while(j<curPathLength){
	   //write(1, "w1\n", 4);//for debugging
     	   newName[i] = currPath[j];
	   i++;
	   j++;
     }
     newName[i] = '/';
     i++;
     j = 0;
     while(j<argLength){
	   //write(1, "w2\n", 4);//for debugging
     	   newName[i] = argv0[j];
	   i++;
	   j++;
     }
     newName[i] = 0;
     return newName;
}

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
			//write(1, "PATH was found!\n", 17);//for debugging
			pathVec = (char**) myTok(&c[5] , ':');//we pass the remainder of the string and the delimiter set to ':'
			 return pathVec;
		      }
	currStr += 1;     
    }
    return pathVec;
}

//helper method used to print the 2D array
void print2DArray(char** array){
//write(1, "w5\n", 4);//for debugging
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

//this method helps the shell support only simple pipes of the type a|b
void checkSimPipes(char** parsedToks, char** pathVector, char** envp){

	int numToks = 0;
	char** cIter = parsedToks;
	while(*cIter != 0){
		numToks++;
		cIter++;
	}
	if(numToks >= 3){
		if(parsedToks[1][0] == '|'){//check if the second token is a pipe like in example.
			int fTokSize = 0;
			char *iter = *parsedToks;
			while(*iter != 0){
				fTokSize++;
				iter++;
				//write(1, "w6\n", 4);//for debugging
			}
			char **fToken = (char**)calloc(2, 1);
			*fToken = (char*) calloc(fTokSize + 1, 1);
			iter = *parsedToks;
			int i = 0;
			while(*iter != 0){//copy the first token
				fToken[0][i] = *iter;
				iter++;
				i++;
				//write(1, "w7\n", 4);//for debugging
			}
			//print2DArray(fToken);//for debugging only
			char** restOfToks = parsedToks + 2;
			//print2DArray(restOfToks);//for debugging only
			int*pipeFds;
			pipeFds = (int*)calloc(2, sizeof(int));
			pipe(pipeFds);
			int pid = fork();
			if(pid == 0){//child
				//write(1, "w8\n", 4);//for debugging
				close(1);//close display, stdout.
				dup(pipeFds[1]);//duplicate the input side of pipe.
				close(pipeFds[0]);
				close(pipeFds[1]);
				
				launcher(0, fToken, pathVector, envp);//fToken
				exit(2);
			}else{//parent
				//write(1, "w9\n", 4);//for debugging
				char buf[100];
				close(0);//close the keyboard, stdin.
				dup(pipeFds[0]);
				close(pipeFds[0]);
				close(pipeFds[1]);
				
				//read(0,buf,sizeof(buf));
				//printf(buf);//for debugging
				launcher(0, restOfToks, pathVector, envp);//restOfToks
				wait(NULL);
				close(0);
				
				//write(0, "\n", 2);
			}
		}else{
		//write(1, "w10\n", 5);//for debugging
		launcher(0, parsedToks, pathVector, envp);
		}
			
	}else{
		//write(1, "w11\n", 5);//for debugging
		launcher(0, parsedToks, pathVector, envp);
	}
}
