// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include "topash.h"

volatile sig_atomic_t bgAllowed = 1;

int main(int argc, char* argv[]) {
	printf("_________________________________________________________________________________________________\n");
	printf(" ________   ______   _______    ______          ______   __    __  ________  __        __        \n");
	printf("|        \\ /      \\ |       \\  /      \\        /      \\ |  \\  |  \\|        \\|  \\      |  \\       \n");
	printf(" \\$$$$$$$$|  $$$$$$\\| $$$$$$$\\|  $$$$$$\\      |  $$$$$$\\| $$  | $$| $$$$$$$$| $$      | $$       \n");
	printf("   | $$   | $$  | $$| $$__/ $$| $$__| $$      | $$___\\$$| $$__| $$| $$__    | $$      | $$       \n");
	printf("   | $$   | $$  | $$| $$    $$| $$    $$       \\$$    \\ | $$    $$| $$  \\   | $$      | $$       \n");
	printf("   | $$   | $$  | $$| $$$$$$$ | $$$$$$$$       _\\$$$$$$\\| $$$$$$$$| $$$$$   | $$      | $$       \n");
	printf("   | $$   | $$__/ $$| $$      | $$  | $$      |  \\__| $$| $$  | $$| $$_____ | $$_____ | $$_____  \n");
	printf("   | $$    \\$$    $$| $$      | $$  | $$       \\$$    $$| $$  | $$| $$     \\| $$     \\| $$     \\ \n");
	printf("    \\$$     \\$$$$$$  \\$$       \\$$   \\$$        \\$$$$$$  \\$$   \\$$ \\$$$$$$$$ \\$$$$$$$$ \\$$$$$$$$ \n");
	printf("_________________________________________________________________________________________________\n");
	fflush(stdout);


	char commandLineInput[MAXLENGTH];
	char path[255] = "";
	char inputFile[255] = "";
	char outputFile[255] = "";
	int keepGoing = 1, bg = 0;
	char* command[512];

	int pid = getpid();
	int exitStatus = -5;
	int* childExitStatus = &exitStatus;

	//set up path so that it can print and look the way I want it to
	chdir(".");
	getcwd(path, sizeof(path));

	struct sigaction si;
	si.sa_handler = SIG_IGN;
	si.sa_flags = 0;
	sigemptyset(&si.sa_mask);
	if (sigaction(SIGINT, &si, NULL) == -1) {
		perror("Couldn't set SIGINT handler");
	}

	struct sigaction sa;
	sa.sa_handler = handleStop;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGTSTP, &sa, NULL) == -1) {
		perror("Couldn't set SIGTSTP handler");
	}

	/*
	* Main loop of the shell 
	* prompts user for input 
	* parses input
	* if input was a built in command run it
	* otherwise have input handled as external command
	*/

	while (keepGoing == 1) {
		if (promptUser(commandLineInput, path) == -1) {
			printf("An error has occured while taking in input. EXITING");
			fflush(stdout);
			break;
		}
		else {
			memset(command, 0, sizeof(*command) * 512); //clear out anything previously in the command array
			memset(inputFile, 0, sizeof(char) * 255); //clear out anything previously in the input file
			memset(outputFile, 0, sizeof(char) * 255); //clear out anything previously in the output file
			processString(pid, commandLineInput, command, inputFile, outputFile, &bg);
			if (!command[0]) {continue;}

			if (strcmp(command[0], "exit") == 0) {
				keepGoing = 0;
			}
			else if (strcmp(command[0], "cd") == 0) {
				if (command[1]) {
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
				if (WIFEXITED(*childExitStatus)) {
					printf("Exit Status: %d\n", WEXITSTATUS(exitStatus));
				}
				else {
					printf("Termination Status: %d\n", WTERMSIG(exitStatus));
				}
				fflush(stdout);
			}
			else { //not a built in command
				runCommand(command, inputFile, outputFile, childExitStatus, &bg, si);
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
void processString(int pid, char* str, char* command[], char* input, char* output, int* bg) {

	*bg = 0;

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

	if (str[strlen(str) - 1] == '&' && str[strlen(str) - 2] == ' ') {
		//str[strlen(str) - 1] = '\0';
		str[strlen(str) - 2] = '\0';
		*bg = 1;
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

		if (strcmp(token, "<") == 0) {
			token = strtok_r(NULL, space, &saveptr);
			strcpy(input, token);
		}
		else if (strcmp(token, ">") == 0) {
			token = strtok_r(NULL, space, &saveptr);
			strcpy(output, token);
		}
		else {

			//Is this good code? No.
			//I have spent days trying to figure out this feature
			//there is definitly better ways to do this. This essentially asks if $$ is within the token
			//if it is then it creates a new string pre and copies everything in token up to $$ into it. 
			//After that if the string has anything after the $$ it copies that to a different string.
			//Then it creates a bigger string full of null pointers and uses sprintf to combine everything.

			char* location = strstr(token, "$$");
			if (location != NULL) { // if the token contains $$
				char pre[250] = "";
				for (int i = 0; i < (location - token); i++) {
					pre[i] = token[i];
				}
				char newToken[300] = "\0";
				if (location[2]) {
					char tmp[253] = "";
					strcpy(tmp, location + 2);
					sprintf(newToken, "%s%d%s", pre, pid, tmp);
				}
				else {
					sprintf(newToken, "%s%d", pre, pid);
				}
				command[i] = newToken;
			}
			else {
				command[i] = token;
			}
		}

		token = strtok_r(NULL, space, &saveptr);
	}
}

/*
* A function mainly meant for debugging purposes
* loops through command[] array and prints out all commands/args
*/

void echoCommand(char* command[]) {
	for (int i = 0; command[i]; i++) {
		printf("%s ", command[i]);
		fflush(stdout);
	}
	printf("\n");
	fflush(stdout);
}

/*
* Any and all external commands go through here
* creates a fork which then uses execvp to run the commands
* handles allowing or not allowing background processes as well as creating them
* handles file redirection aswell
*/

void runCommand(char* command[], char* input, char* output, int* childExitStatus, int* bg, struct sigaction si) {
	pid_t spawnPid = -5;

	int sourceFD, targetFD, result;

	//handle background processes redirection if not given specific redirection instructions
	if (*bg == 1 && bgAllowed == 1 && strcmp(input, "") == 0) {
		input = "/dev/null";
	}
	if (*bg == 1 && bgAllowed == 1 && strcmp(output, "") == 0) {
		output = "/dev/null";
	}

	//pain
	spawnPid = fork();
	switch (spawnPid) {
		case -1: {perror("Something ain't right\n"); exit(1); break;}
		case 0: {

			//handle SIGINT for processes
			if (*bg == 1 && bgAllowed == 1) {
				si.sa_handler = SIG_DFL;
				if (sigaction(SIGINT, &si, NULL) == -1) {
					perror("Couldn't set SIGTSTP handler");
				}
			}

			//Uses loop to increment i to the amount of commands
			int i = 0;
			for (i; command[i]; i++) {}
			command[i] = NULL;

			//handles input redirection
			if (strcmp(input, "")) {
				sourceFD = open(input, O_RDONLY);
				if (sourceFD == -1) { perror("source open()"); exit(1); }
				result = dup2(sourceFD, 0); //set STDIN to point to same thing that sourceFD is pointing to
				fcntl(sourceFD, F_SETFD, FD_CLOEXEC);
			}

			//handles output redirection
			if (strcmp(output, "")) {
				targetFD = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0666); //let anyone read and write 
				if (targetFD == -1) { perror("target open()"); exit(1); }
				result = dup2(targetFD, 1); //set STDOUT to point to same thing that sourceFD is pointing to
				fcntl(targetFD, F_SETFD, FD_CLOEXEC);
			}

			//run the actual command
			if (execvp(command[0], command) < 0) {
				perror("exec failure!");
				exit(1);
			}

		}
		default: {

			/*
			* Handles everything from the parent side
			* this includes handling making the process a background process
			* as well as waiting for the termination of a process and exit status
			*/

			if (*bg == 1 && bgAllowed == 1) {
				pid_t actualPid = waitpid(spawnPid, childExitStatus, WNOHANG);
				printf("BG CHILD : %d === CREATED\n", spawnPid);
				fflush(stdout);
			}
			else {
				pid_t actualPid = waitpid(spawnPid, childExitStatus, 0);
			}

			while (0 < (spawnPid = waitpid(-1, childExitStatus, WNOHANG))) {
				printf("BG CHILD : %d === TERMINATED\n", spawnPid);
				
				if (WIFEXITED(childExitStatus)) {
					printf("Exit Status: %d\n", WEXITSTATUS(*childExitStatus));
				}
				else {
					printf("Termination Status: %d\n", WTERMSIG(*childExitStatus));
				}
				fflush(stdout);
			}


		}
	}
}

void handleStop(int signo) {
	if (bgAllowed == 1) {
		char* painAndSuffering = "\nStart Foreground-only mode\n";
		write(STDOUT_FILENO, painAndSuffering, 28);
		bgAllowed = 0;
	}
	else {
		char* sufferingAndPain = "\nEnd Foreground-only mode\n";
		write(STDOUT_FILENO, sufferingAndPain, 26);
		bgAllowed = 1;
	}
}