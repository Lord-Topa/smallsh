//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
// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include "smallsh.h"


int main(int argc, char* argv[]) {
	//allows the maximum command line input to be 2048 characters
	char commandLineInput[MAXLENGTH];
	char inputFile[255] = "";
	char outputFile[255] = "";
	int keepGoing = 1, bg = 0;
	char* command[512];
	pid_t spawnPid = -5;
	int childExitStatus = -5;


	//int bg = 0;
	while (keepGoing == 1) {
		if (promptUser(commandLineInput) == -1) {
			printf("An error has occured while taking in input. EXITING");
			break;
		}
		else {
			processString(commandLineInput, command, inputFile, outputFile, &bg);
			echoCommand(command);
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
*	Make commands starting with # a comment
*	remove \n from the user pressing enter
*	check to make sure string is not empty
*	split string up into multiple smaller strings to check for args
*/
void processString(char* str, char* command[], char* input, char* output, int* bg) {
	
	memset(command, 0, sizeof(*command) * 512); //clear out anything previously in the command array

	if (str[0] == 35) { //if the line starts with # it is a comment and should be disregarded
		return;
	}

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

		/*
		* The documentation gives us 4 special characters to handle, the first one I will handle is &
		* this is because & is used to mark the end of a command and tell us that the command should be 
		* run in the back ground. Since we know that it will be at the end of the command we do not need to 
		* check anything after the &
		* 
		* The next characters to handle are < and > which are used to signify input and output files 
		* respectively
		*/


		if (strcmp(token, "&") == 0) {
			*bg = 1;
			break;
		}
		else if (strcmp(token, "<") == 0) {
			token = strtok_r(NULL, space, &saveptr);
			strcpy(input, token);
		}
		else if (strcmp(token, ">") == 0) {
			token = strtok_r(NULL, space, &saveptr);
			strcpy(output, token);
		}

		command[i] = token;
		token = strtok_r(NULL, space, &saveptr);
	}
}

void echoCommand(char* command[]) {
	for (int i = 0; command[i]; i++) {
		printf("%s ", command[i]);
		fflush(stdout);
	}
	printf("\n");
	fflush(stdout);
}