#ifndef __SMALLSH_H
#define __SMALLSH_H

#define MAXLENGTH 2048

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <time.h>

int promptUser(char* storage);
void processString(char* str, char* command[], char* input, char* output, int* bg);
void echoCommand(char* command[]);

#endif