#ifndef __SMALLSH_H
#define __SMALLSH_H

#define MAXLENGTH 2048

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

int promptUser(char* storage, char* path);
void processString(int pid, char* str, char* command[], char* input, char* output, int* bg);
void echoCommand(char* command[]);
void runCommand(char* command[], char* input, char* output, int* childExitStatus, int* bg, struct sigaction si);
void handleStop(int signo);

#endif