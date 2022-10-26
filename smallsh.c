//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
// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include "smallsh.h"


int main(int argc, char* argv[]) {
	char* commandLineInput = (char*)malloc(MAXLENGTH * sizeof(char)); //allows the maximum command line input to be 2048 characters
	int keepGoing = 1;
	char* command[512];
	while (keepGoing == 1) {
		if (promptUser(commandLineInput) == -1) {
			printf("An error has occured while taking in input. EXITING");
			break;
		}
		else {
			processString(commandLineInput, command);
			for (int i = 0; i < 4; i++) {
				printf("%s\n", command[i]);
			}
			//printf("An error has occured while processing your input. REPROMPTING");
		}
	}
	
	return 0;
}

int promptUser(char* storage) {
	printf(": ");
	fflush(stdout);
	fgets(storage, MAXLENGTH, stdin);
	if (storage) {
		//printf("0");
		return 0;
	}
	else {
		printf("-1");
		return -1;
	}
}

/*
* Will take in strings and prepare them for usage in the shell
* Things will need be done:
*	remove \n from the user pressing enter
*	check to make sure string is not empty
*	split string up into multiple smaller strings to check for args
*/
void processString(char* str, char* command[]) {
	


	//remove \n
	for (int i = 0; i < strlen(str); i++) { //just replace the newline char with a null terminator
		if (str[i] == '\n') {
			str[i] = '\0';
			break;
		}
	}

	//make sure the string isn't 'empty' (just lacking characters)
	if (strcmp(str, "") == 0) {
		printf("EMPTY STRING");
	}
	
	//split string into smaller strings for args
	char* saveptr;
	char* space = " ";
	char* token = strtok_r(str, space, &saveptr);
	for (int i = 0; token; i++) {
		//strcpy(command[i], token);
		command[i] = token;
		token = strtok_r(NULL, space, &saveptr);
	}
}