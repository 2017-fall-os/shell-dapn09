#ifndef tokenizer_header
#define tokenizer_header

void print2DArray(char** array);
void freeArray(char** array);
char ** myTok(char *str, char delim);
int launcher(int argc, char *argv[], char *envp[]);
char ** getPath(char** envp);
int tokenLen(char *tok);

#endif
