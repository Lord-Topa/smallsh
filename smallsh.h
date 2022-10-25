
#ifndef __SMALLSH_H
#define __SMALLSH_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

int promptUser(char* storage);
void processString(char* str);

#endif