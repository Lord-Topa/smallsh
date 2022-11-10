# Topa Shell



## CONTENTS: 
	
* [Instructions](https://github.com/Lord-Topa/smallsh/blob/main/README.md#instructions)

* [Description](https://github.com/Lord-Topa/smallsh/blob/main/README.md#description)

* [In-depth Breakdown of Project Requirements:](https://github.com/Lord-Topa/smallsh/blob/main/README.md#in-depth-breakdown-of-project-requirments)
	1) [Provide a prompt for running commands](https://github.com/Lord-Topa/smallsh/blob/main/README.md#provide-a-prompt-for-running-commands)
	2) [Handle blank lines and comments](https://github.com/Lord-Topa/smallsh/blob/main/README.md#handle-blank-lines-and-comments)
	3) [Provide expansion for the variable $$](https://github.com/Lord-Topa/smallsh/blob/main/README.md#provide-expansion-for-the-variable-)
	4) [Execute 3 commands exit, cd, and status via code built into the shell](https://github.com/Lord-Topa/smallsh/edit/main/README.md#4-execute-3-commands-via-code-built-into-the-shell-exit-cd-status)
	5) [Execute other commands by creating new processes using a function from the exec family of functions](https://github.com/Lord-Topa/smallsh/blob/main/README.md#execute-3-commands-via-code-built-into-the-shell-exit-cd-status)
	6) [Support input and output redirection](https://github.com/Lord-Topa/smallsh/blob/main/README.md#support-input-and-output-redirection)
	7) [Support running commands in foreground and background processes](https://github.com/Lord-Topa/smallsh/blob/main/README.md#support-running-commands-in-foreground-and-background-processes
	8) [Implement custom handlers for 2 signals, SIGINT and SIGTSTP](https://github.com/Lord-Topa/smallsh/blob/main/README.md#implement-custom-handlers-for-2-signals-sigint-and-sigtstp)

* [Sources Used](https://github.com/Lord-Topa/smallsh/blob/main/README.md#sources-used)



## INSTRUCTIONS: 
	


*For best experience have terminal maximized or fullscreen*

**To run the shell use these commands in this order:**
		
>make clean; make
		
>./topash

**General Shell Syntax:**

* Commmands should follow the syntax of...

	*command [arg1 arg2 ...] [< input_file] [> output_file] [&]*

	…where items in square brackets are optional.
	
* Lines starting with *#* are considered comments and will be ignored by the shell
* A "*&*" at the end of a command will mark that command as one to be run in the background
* A "*<*" followed by a space and then a filename will mark that file as the input file for a command
* A "*>*" followed by a space and then a filename will mark that file as the output file for a command

**Built in Shell Commands:**
			
>cd - Will navigate shell to home directory

>cd <PATH> - Will navigate shell to PATH 
		
>CTRL+Z - keyboard shortcut that on most systems will enter or exit foreground-only mode

>status - Will return exit status of most recent program, if no programs run will return exit status 0

>exit - Will exit the shell



## DESCRIPTION: 



A small shell I devolped in just under 2 weeks (10/25/2022 - 11/7/2022) for project 3 in my Operating Systems class.

**The requirements for this project were:**

>1) Provide a prompt for running commands

>2) Handle blank lines and comments, which are lines beginning with the # character

>3) Provide expansion for the variable $$

>4) Execute 3 commands exit, cd, and status via code built into the shell

>5) Execute other commands by creating new processes using a function from the exec family of functions

>6) Support input and output redirection

>7) Support running commands in foreground and background processes

>8) Implement custom handlers for 2 signals, SIGINT and SIGTSTP

## In-Depth Breakdown of Project Requirments:
------------------
### PROVIDE A PROMPT FOR RUNNING COMMANDS:
------------------


* Use the colon : symbol as a prompt for each command line. 

* The general syntax of a command line is:
	*command [arg1 arg2 ...] [< input_file] [> output_file] [&]*
	…where items in square brackets are optional.

* Assume that a command is made up of words separated by spaces.

* The special symbols <, > and & are recognized, but they must be surrounded by spaces like other words.

* If the command is to be executed in the background, the last word must be &. If the & character appears anywhere else, just treat it as normal text.

* If standard input or output is to be redirected, the > or < words followed by a filename word must appear after all the arguments. Input redirection can appear before or after output redirection.

* The shell does not need to support any quoting; so arguments with spaces inside them are not possible. We are also not implementing the pipe "|" operator.

* The shell must support command lines with a maximum length of 2048 characters, and a maximum of 512 arguments.

* Do not need to do any error checking on the syntax of the command line.


------------------
### HANDLE BLANK LINES AND COMMENTS
------------------


* The shell should allow blank lines and comments.

* Any line that begins with the # character is a comment line and should be ignored. Mid-line comments, such as the C-style //, will not be supported.

* A blank line (one without any commands) should also do nothing.

* The shell should just re-prompt for another command when it receives either a blank line or a comment line.


------------------
### PROVIDE EXPANSION FOR THE VARIABLE $$
------------------


* The shell must expand any instance of "$$" in a command into the process ID of the smallsh itself. The shell does not otherwise perform variable expansion. 


------------------
### EXECUTE 3 COMMANDS VIA CODE BUILT INTO THE SHELL (exit, cd, status)
------------------


* Do not have to support input/output redirection for these built in commands

* These commands do not have to set any exit status.

* If the user tries to run one of these built-in commands in the background with the & option, ignore that option and run the command in the foreground anyway (i.e. don't display an error, just run the command in the foreground).

* exit : exits the shell, takes no args, when run shell must kill any other processes or jobs that the shell has started before it terminates.

* cd : changes working directory of shell, if no args changes to directory specified in the HOME enviroment variable, otherwise go to path specified (relative or absolute)

* status : prints out either the exit status or the terminating signal of the last foreground process ran by the shell, if run before any foreground command then return exit status 0, should ignore built in commands


------------------
### EXECUTE OTHER COMMANDS BY CREATING NEW PROCESSES USING A FUNCTION FROM THE EXEC FAMILY OF FUNCTIONS
------------------


* Shell will execute any commands other than the 3 built-in command by using fork(), exec() and waitpid()

* Whenever a non-built in command is received, the parent (i.e., smallsh) will fork off a child.

* The child will use a function from the exec() family of functions to run the command.

* The shell should use the PATH variable to look for non-built in commands, and it should allow shell scripts to be executed

* If a command fails because the shell could not find the command to run, then the shell will print an error message and set the exit status to 1

* A child process must terminate after running a command (whether the command is successful or it fails).


------------------
### SUPPORT INPUT AND OUTPUT REDIRECTION
------------------


* Must do any input and/or output redirection using dup2(). The redirection must be done before using exec() to run the command.

* An input file redirected via stdin should be opened for reading only; if the shell cannot open the file for reading, it should print an error message and set the exit status to 1 (but don't exit the shell).

* Similarly, an output file redirected via stdout should be opened for writing only; it should be truncated if it already exists or created if it does not exist. If the shell cannot open the output file it should print an error message and set the exit status to 1 (but don't exit the shell).

* Both stdin and stdout for a command can be redirected at the same time.


------------------
### SUPPORT RUNNING COMMANDS IN FOREGROUND AND BACKGROUND PROCESSES
------------------


**FOREGROUND**

* Any command without an & at the end must be run as a foreground command and the shell must wait for the completion of the command before prompting for the next command.
For such commands, the parent shell does NOT return command line access and control to the user until the child terminates.

**BACKGROUND**

* Any non built-in command with an & at the end must be run as a background command and the shell must not wait for such a command to complete. 
For such commands, the parent must return command line access and control to the user immediately after forking off the child.

* The shell will print the process id of a background process when it begins.

* When a background process terminates, a message showing the process id and exit status will be printed. This message must be printed just before the prompt for a new command is displayed.

* If the user doesn't redirect the standard input for a background command, then standard input should be redirected to /dev/null

* If the user doesn't redirect the standard output for a background command, then standard output should be redirected to /dev/null


------------------
### IMPLEMENT CUSTOM HANDLERS FOR 2 SIGNALS, SIGINT AND SIGTSTP
------------------


**SIGINT**

* The shell, i.e., the parent process, must ignore SIGINT

* Any children running as background processes must ignore SIGINT

* A child running as a foreground process must terminate itself when it receives SIGINT

	* The parent must not attempt to terminate the foreground child process; instead the foreground child (if any) must terminate itself on receipt of this signal.

	* If a child foreground process is killed by a signal, the parent must immediately print out the number of the signal that killed it's foreground child process (see the example) before prompting the user for the next command.

**SIGTSTP**
* A child, if any, running as a foreground process must ignore SIGTSTP.

* Any children running as background process must ignore SIGTSTP.

* When the parent process running the shell receives SIGTSTP:

	* The shell must display an informative message immediately if it's sitting at the prompt, or immediately after any currently running foreground process has terminated

	* The shell then enters a state where subsequent commands can no longer be run in the background.

	* In this state, the & operator should simply be ignored, i.e., all such commands are run as if they were foreground processes.

* If the user sends SIGTSTP again, then the shell will:

	* Display another informative message immediately after any currently running foreground process terminates

	* The shell then returns back to the normal condition where the & operator is once again honored for subsequent commands, allowing them to be executed in the background.



## SOURCES USED:



Used to generate ASCII Art (edits made to art for compatibility):
	
* https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20

Used for help on signal handlers: 

* https://stackoverflow.com/questions/40098170/handling-sigtstp-signals-in-c 

* https://www.geeksforgeeks.org/signals-c-language/

Used for file permissions help:

* https://chmod-calculator.com

Used for general help:

* https://linux.die.net

* Lecture Slides
