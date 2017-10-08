#include "helpers.h"
#include <stdio.h>
#include "myTok.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

char** enVars;

void main(int argc, char **argv, char**envp){

  //create a persistent prompt.
  int ex = 0;
  //was used in tokenizer, serves as a tool now.
  char dlt[1] = {' '}; 
  char* prompt;
  enVars = envp;//enVars is now the official environment.
  char** comsByNewLine;
  char** coms;
  
  while(!ex){
    
    if(!enVarExist("PS1", enVars))//if PS1 does not exist, create it.
    enVars = createEnVar("PS1", "$ ", enVars);

    prompt = getEnVar("PS1", enVars);
    if(prompt != 0)//only print prompt if exists.
    fprintf(stderr, "%s", prompt);//write prompt to the screen.


    char* buffer = (char*)calloc(128, sizeof(char));
    int ans = read(0, buffer, 128);//read input.

    if(ans < 0){//if there is an error in the reading process.
      fprintf(stderr, "%s\n", strerror(errno));
      exit(1);
    }

    if(ans == 0){//if EOF read, exit.
      fprintf(stderr, "%s\n", strerror(errno));
      exit(1);
    }
	  
    if((buffer[0] == 0)||(buffer[0] == '\n')){
	   //if(buffer[0] == '\n')
           //printf("buffer[0] is: %c\n", buffer[0]); 
	   continue; //if there is no input, start again.
    }
	  
    
    //to check for the keyword to exit the tokenizer.
    if(ans > 0){

    coms = myTok(buffer , '\n');
      
    comsByNewLine = coms;//tokenize by '\n'
    while(*comsByNewLine){//iterate through the statements
    char delimiter = dlt[0];
    char ** parsedToks = myTok(*comsByNewLine, delimiter);
    char ** pathVector = getPath(enVars);
    int size1stCom = tokenLen(parsedToks[0]);
    //check for exit key word
    if(strcmp(parsedToks[0], "exit") == 0){
        write(1, "Bye!!!\n", 7);
        ex = 1;
    }

     //check for printing environment vars.    
    if(strcmp(parsedToks[0], "envp") == 0){
        print2DArray(enVars);//print environmental variables
        //free(*comsByNewLine);//might cause probs
        break;
    }

    if(strcmp(parsedToks[0], "export") == 0){
      
      	enVars = setEnVar(parsedToks[1], enVars);//change variable and update enVars.
	
	//free(*comsByNewLine);//might cause probs
        break;
    }

     //check for changing directory
    if(size1stCom == 2)    
       if(strcmp(parsedToks[0], "cd") == 0){
	 int res = chdir(parsedToks[1]); //change directory

	      if(res != 0){
	      	fprintf(stderr, "%s\n", strerror(errno));
	      }
	//free(*comsByNewLine);//might cause probs
       break;
       }

      char* trimCom = rmTailSpaces(*comsByNewLine);//for debugging
      //if the word entered is not exit then pass the string to myTok.
      if(!ex){
      //print2DArray(parsedToks);//for debugging purposes only.

	analyzer(trimCom, pathVector, enVars);
	//launcher(0, parsedToks, pathVector, envp);
      }
      freeArray(parsedToks);
      free(trimCom);

      comsByNewLine++;//advance
    }
    
    }
    
    
  }
  
}
