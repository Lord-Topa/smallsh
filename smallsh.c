//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
// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include "smallsh.h"


int main(int argc, char* argv[]) {
	//allows the maximum command line input to be 2048 characters
	char commandLineInput[MAXLENGTH];
	char path[255] = "";
	char inputFile[255] = "";
	char outputFile[255] = "";
	int keepGoing = 1, bg = 0;
	char* command[512];

	int childExitStatus = -5;
	pid_t spawnPid = -5;

	//set up path so that it can print and look the way I want it to
	chdir(".");
	getcwd(path, sizeof(path));

	while (keepGoing == 1) {
		if (promptUser(commandLineInput, path) == -1) {
			printf("An error has occured while taking in input. EXITING");
			break;
		}
		else {
			memset(command, 0, sizeof(*command) * 512); //clear out anything previously in the command array
			memset(inputFile, 0, sizeof(char) * 255); //clear out anything previously in the input file
			memset(outputFile, 0, sizeof(char) * 255); //clear out anything previously in the output file
			processString(commandLineInput, command, inputFile, outputFile, &bg);
			
			if (!command[0]) {continue;}
			//echoCommand(command);

			//Exit and cd were pretty simple to implement so I got thos out of the way

			if (strcmp(command[0], "exit") == 0) {
				keepGoing = 0;
			}
			else if (strcmp(command[0], "cd") == 0) {
				if (command[1]) {
					sprintf(path, "%s/%s", path, command[1]);
					if (chdir(command[1]) == -1) {
						printf("!!! Could not navigate to '%s' ERROR MESSAGE: %s!!!\n", path, strerror(errno));
					}
					getcwd(path, sizeof(path));
				}
				else {
					char* home = getenv("HOME");
					sprintf(path, "%s", home);
					chdir(path);
					getcwd(path, sizeof(path));
				}
			}
			else if (strcmp(command[0], "status") == 0) {
				//Return status
				if (WIFEXITED(childExitStatus)) {
					printf("Exit Status: %d\n", WEXITSTATUS(childExitStatus));
				}
				else {
					printf("Termination Status: %d\n", WTERMSIG(childExitStatus));
				}
			}
			else { //not a built in command
				//do something
				runCommand(command, &bg, inputFile, outputFile, &childExitStatus);
			}
		}
	}

	return 0;
}

int promptUser(char* storage, char* path) {
	//display the current path and prompt
	printf("%s", path);
	printf(": ");
	fflush(stdout);

	fgets(storage, MAXLENGTH, stdin);
	if (storage) {
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
	
	//memset(command, 0, sizeof(*command) * 512); //clear out anything previously in the command array

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

	//make sure the string isn't 'empty'
	if (strcmp(str, "") == 0) {
		return;
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
		else {
			command[i] = token;
		}

		//command[i] = token;
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

void runCommand(char* command[], int* bg, char* input, char* output, int* childExitStatus) {
	pid_t spawnPid = -5;
	//int childExitStatus = -5;

	/*
	* still needs to handle background stuff and child exit status stuff
	*/
	int sourceFD, targetFD, result;

	spawnPid = fork();
	switch (spawnPid) {
		case -1: {perror("Something ain't right\n"); exit(1); break;}
		case 0: {
			//printf("CHILD(%d): Sleeping for 1 second\n", getpid());
			//sleep(1);
			int i = 0;
			for (i; command[i]; i++) {}
			//printf("%d\n", i);
			command[i] = NULL;
			//execlp("ls", "ls", "-a", NULL);

			
			if (strcmp(input, "")) {
				//printf("%s\n", input)
				sourceFD = open(input, O_RDONLY);
				if (sourceFD == -1) { perror("source open()"); exit(1); }
				result = dup2(sourceFD, 0); //set STDIN to point to same thing that sourceFD is pointing to
				fcntl(sourceFD, F_SETFD, FD_CLOEXEC);
			}

			if (strcmp(output, "")) {
				targetFD = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0666); //let anyone read and write 
				if (targetFD == -1) { perror("target open()"); exit(1); }
				result = dup2(targetFD, 1); //set STDOUT to point to same thing that sourceFD is pointing to
				fcntl(targetFD, F_SETFD, FD_CLOEXEC);
			}

			if (execvp(command[0], command) < 0) {
				perror("exec failure!");
				exit(1);
			}
			//exit(2); break;
		}
		default: {
			//printf("PARENT(%d): Sleeping for 2 seconds\n", getpid());
			//sleep(2);
			//printf("PARENT(%d): Wait()ing for child(%d) to terminate\n", getpid(), spawnPid);
			pid_t actualPid = waitpid(spawnPid, &childExitStatus, 0);
			//printf("PARENT(%d): Child(%d) terminated, Exiting!\n", getpid(), actualPid);
			//exit(0); break;
		}
	}
}
