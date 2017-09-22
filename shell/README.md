# Shell Lab #

In this lab we build a simple shell, a shell is essentially the program that manages command line input from the user in any common non-gui system, in windows the shell is invoked by executing ```cmd```, In Unix based systems the shell might be called ```bash```.  

## How To Run ##

In order to run this program, from the command line just type ``` make ``` and then hit enter, this will compile the files into an executable. In order to run the executable, just type ``` ./shell ``` and hit enter. The program should start and the prompt line should now read ```$ ``` and wait for input.

- To execute programs with the shell, simply enter the command and arguments (ie. ```echo hello world```) and hit enter, like a normal shell. You could also specify the full name of the file or command to execute (ie. ```/usr/bin/echo hello world```). 
- To exit the shell, type ```exit``` and hit enter. This should take you back to your system's shell.

###### Files ######

 - myTok.c: This file contains the crucial tokenizer the path finder and the launcher methods for the shell.
 - myTok.h: This file contains the function signatures for myTok, findPath & launcher.
 - shell.c: This file contains main, it serves as a wrapper for the methods in the myTok.c file.
