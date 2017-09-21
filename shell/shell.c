#include <stdio.h>
#include "myTok.h"
#include <unistd.h>
#include <stdlib.h>


void main(){

  //STEP 1: create a persistent prompt
  int exit = 0;
  char dlt[1] = {' '}; 

  while(!exit){

    write(1, "$ ", 2);//write prompt to the screen.
    char* buffer = (char*)calloc(128, sizeof(char));
    int ans = read(0, buffer, 128);

   
    if(ans < 0){//if there is an error in the reading process.
      write(1, "Ooops!!! There was an error while reading the input.\n", 53);
    }
    
    //to check for the keyword to exit the tokenizer.
    if(ans > 0){
      if(buffer[0] == 'e')
	if(buffer[1] == 'x')
	  if(buffer[2] == 'i')
	    if(buffer[3] == 't'){
	      write(1, "Bye!!!\n", 7);
	      exit = 1;
	    }

      if(buffer[0] == 'd')
	if(buffer[1] == 'l')
	  if(buffer[2] == 't'){
	    write(1, "Enter the new delimiter:", 24);
	    read(0, dlt, 1);
	    write(1, "New delimiter set", 17);
	    continue;
	  }
	    

      //if the word entered is not exit then pass the string to myTok.
      if(!exit){
	char delimiter = dlt[0];
	char ** parsedToks = myTok(buffer, delimiter);
	print2DArray(parsedToks);
	freeArray(parsedToks);
	
      }

      free(buffer);
    }
    
  }
  
  
}