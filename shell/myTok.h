#ifndef tokenizer_header
#define tokenizer_header

char ** myTok(char *str, char delim);
int launcher(int argc, char *argv[], char* path[], char *envp[]);
void analyzer(char* origString, char** pathVector, char** envp);
void runWithPipes(char* process);
void runForeGround(int num, char* process);
void runBackGround(int num, char* process);
void prepGusintah(char*process);
#endif
