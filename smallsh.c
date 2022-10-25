// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include "smallsh.h"

int main(int argc, char* argv[]) {
	char* commandLineInput = (char*)malloc(2048 * sizeof(char)); //allows the maximum command line input to be 2048 characters
	int keepGoing = 1;
	while (keepGoing == 1) {
		if (promptUser(commandLineInput) == -1) {
			printf("An error has occured while taking in input. EXITING");
			break;
		}
		else {
			processString(commandLineInput);
			//printf("An error has occured while processing your input. REPROMPTING");
		}
	}
	
	return 0;
}

int promptUser(char* storage) {
	printf(":");
	scanf("%s", &storage);
	if (storage) {
		return 0;
	}
	else {
		return -1;
	}
}

void processString(char* str) {
	printf("%s\n", str);
}