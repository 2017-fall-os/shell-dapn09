#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "myTok.h"
#include "helpers.h"
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

int countTokens(char** array){
//write(1, "w5\n", 4);//for debugging
  char** tokens = array;
  int count = 0;
  
  while(*tokens != 0){
    count++;
    tokens++;
  }
  return count;
}

int ocurrencesOf(char needle, char *hayStack){

  char *iter = hayStack;
  int count = 0;
  
  while(*iter != 0){
    if(*iter == needle){
      count++;
    }
    iter++;
  }
  return count;
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

char * rmTailSpaces(char* buffer){

  int i = tokenLen(buffer) - 1;
  while((buffer[i] == '\n')||(buffer[i] == ' ')){
    buffer[i] = 0;
    i--;
  }
  return buffer;
} 
