#ifndef helpers_header
#define helpers_header

void print2DArray(char** array);
void freeArray(char** array);
char ** getPath(char** envp);
int tokenLen(char *tok);
char *prepName(char* currPath, char* argv0);
int countTokens(char** array);
int ocurrencesOf(char needle, char *hayStack);
char *rmTailSpaces(char* buffer);
char* getEnVar(char* varName, char** enVars);
int indexOf(char needle, char* hayStack);
char** setEnVar(char* nameAndVal, char** enVars);
#endif
