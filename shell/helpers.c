#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "myTok.h"
#include "helpers.h"
#include <errno.h>

//get the value of an environmental variable.
char* getEnVar(char* varName, char** enVars){

  char** iter = enVars;
  int givenNameSize = tokenLen(varName);
  int givenValSize = 0;
  int varNameSize = 0;
  int cVarSize = 0;
  int found = 0; //effectively boolean.
  int matches = 0;
  int i = 0;
  while(*iter){
    cVarSize = tokenLen(*iter);//we must know if the name given exists in enVars.
    varNameSize = indexOf('=', *iter);
    if(varNameSize == givenNameSize){//if we find a match in sizes, then check if thyre equal.
      for(i = 0; i < givenNameSize; i++){
	if((*iter)[i] == varName[i]){
	  matches++;
	}
      }
      if(matches == givenNameSize){
	  found = 1;
	  break;//if found, no need to keep searching.
	  matches = 0;
      }else{
	  matches = 0;
      }
    }
  iter++;
  }

  char** value;//the default value to return in case var not found.
    
  //iter holds the string that matched, i holds the index of \"   
  if(found){
    
    value = myTok((*iter) + i + 1, '\"');
      
  }else{
    fprintf(stderr, "ERROR: environmental variable \"%s\" not found.\n", varName);
    value = (char**)calloc(2, sizeof(char*));
    *value = (char*)calloc(2, sizeof(char));
    value[0] = "";
  }
  return value[0];
}

//return the index of the char if found, -1 if not.
int indexOf(char needle, char* hayStack){

  char *iter = hayStack;
  int index = 0;
  while(*iter){
    if(*iter == needle){
      return index;
    }
    index++;
    iter++;
  }
  return -1;
}

//set an environmental variable into the given value.
char** setEnVar(char* nameAndVal, char** enVars){

  //first we must tokenize nameAndVal.
  char** naVal = myTok(nameAndVal, '=');
  if(countTokens(naVal) != 2){
    fprintf(stderr, "ERROR: please use the format name=\"val\" for param, no spaces.\n");
    return enVars;
  }

  int numVars = 0;
  char** iter = enVars;
  while(*iter){
    numVars++;
    iter++;
  }
  //allocate new vector's space.
  char** newEnVars = (char**) calloc(numVars + 1, sizeof(char*));
  char** newIter = newEnVars;
  iter = enVars;
  int givenNameSize = tokenLen(naVal[0]);
  int givenValSize = 0;
  int varNameSize = 0;
  int cVarSize = 0;
  int found = 0; //effectively boolean.
  int matches = 0;
  while(*iter){
    cVarSize = tokenLen(*iter);//we must know if the name given exists in enVars.
    varNameSize = indexOf('=', *iter);
    if(varNameSize == givenNameSize){//if we find a match in sizes, then check if thyre equal.
      for(int i = 0; i < givenNameSize; i++){
	if((*iter)[i] == naVal[0][i]){
	  matches++;
	}
      }
      if(matches == givenNameSize){
	  found = 1;
	  matches = 0;
      }else{
	  matches = 0;
      }
    }
    //the new string.
    if(found){//if found, allocate space accordingly
     givenValSize = tokenLen(naVal[1]); 
     //space will be allocated for: varNameSize + = + givenValSize + null.
     *newIter = (char*) calloc(varNameSize + givenValSize + 2, sizeof(char));
     int j = 0;
     for(; j<varNameSize; j++){//first copy name
       *newIter[j] = *iter[j];
     }
     *newIter[j] = '=';//enter the equal sign
     for(int i = 0; i<givenValSize; i++){//thencopy new value

       *newIter[j] = naVal[1][i];
       j++;
     }
    }
    else{//copy the whole string as it is.
     *newIter = (char*) calloc(cVarSize + 1, sizeof(char));  
     for(int i = 0; i<cVarSize; i++){
       *newIter[i] = *iter[i];
     }
    }
    newIter++;
    iter++;
  }
  if(!found) fprintf(stderr, "ERROR: Given variable name not found, vars unchanged.\n");
  freeArray(naVal);
  return newEnVars;
}


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
