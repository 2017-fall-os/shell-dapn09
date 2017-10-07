#include "helpers.h"
#include <stdio.h>
#include "myTok.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


void main(int argc, char **argv, char**envp){

  //create a persistent prompt.
  int ex = 0;
  //was used in tokenizer, serves as a tool now.
  char dlt[1] = {' '}; 
  

  while(!ex){

    write(1, "$ ", 2);//write prompt to the screen.
    char* buffer = (char*)calloc(128, sizeof(char));
    int ans = read(0, buffer, 128);

   
    if(ans < 0){//if there is an error in the reading process.
      fprintf(stderr, "%s\n", strerror(errno));
      exit(1);
    }

    if(ans == 0){//if EOF read, exit.
      fprintf(stderr, "%s\n", strerror(errno));
      exit(1);
    }
	  
    if((buffer[0] == 0)||(buffer[0] == '\n')||(buffer[0] == EOF)){
	   //if(buffer[0] == '\n')
           //printf("buffer[0] is: %c\n", buffer[0]); 
	   continue; //if there is no input, start again.
    }
	  
    
    //to check for the keyword to exit the tokenizer.
    if(ans > 0){	    
    char delimiter = dlt[0];
    char ** parsedToks = myTok(buffer, delimiter);
    char ** pathVector = getPath(envp);
    int size1stCom = tokenLen(parsedToks[0]);
    //check for exit key word
    if(size1stCom > 3)    
      if(parsedToks[0][0] == 'e')
	if(parsedToks[0][1] == 'x')
	  if(parsedToks[0][2] == 'i')
	    if(parsedToks[0][3] == 't'){
	      write(1, "Bye!!!\n", 7);
	      ex = 1;
	    }

      /*if(buffer[0] == 'd')
	if(buffer[1] == 'l')
	  if(buffer[2] == 't'){
	    write(1, "Enter the new delimiter:", 24);
	    read(0, dlt, 1);
	    write(1, "New delimiter set\n", 19);
	    free(buffer);
	    continue;
	  }*/
     //check for printing environment vars.    
     if(size1stCom > 4)    
      if(parsedToks[0][0] == 'e')//for testing purposes only, remove for final submission
	if(parsedToks[0][1] == 'n')
	  if(parsedToks[0][2] == 'v')
	    if(parsedToks[0][3] == 'p'){
	      print2DArray(envp);//print environmental variables
	      free(buffer);
	      continue;
      }
	    
     //check for changing directory
     if(size1stCom == 2)    
       if(parsedToks[0][0] == 'c')//for testing purposes only, remove for final submission
	 if(parsedToks[0][1] == 'd'){
	      int res = chdir(parsedToks[1]); //change directory 
	      if(res != 0){
	      	fprintf(stderr, "%s\n", strerror(errno));
	      }
		//char cwd[1024];
		//fprintf(stderr, "The current dir is: %s\n", getcwd(cwd, sizeof(cwd)));
	      free(buffer);
	      continue;
         }

      char* trimBuff = rmTailSpaces(buffer);//for debugging
      //if the word entered is not exit then pass the string to myTok.
      if(!ex){
	      //print2DArray(parsedToks);//for debugging purposes only.

	analyzer(trimBuff, pathVector, envp);
	//launcher(0, parsedToks, pathVector, envp);
      }
      freeArray(parsedToks);
      free(trimBuff);
    }
    
  }
  
  
}
