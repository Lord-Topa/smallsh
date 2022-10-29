# smallsh
A small shell I devolped in just under 2 weeks (10/25/2022 - 11/7/2022) for project 3 in my Operating Systems class.

Some of the requirements for this project were:

1.) Provide a prompt for running commands

2.) Handle blank lines and comments, which are lines beginning with the # character

3.) Provide expansion for the variable $$

4.) Execute 3 commands exit, cd, and status via code built into the shell

5.) Execute other commands by creating new processes using a function from the exec family of functions

6.) Support input and output redirection

7.) Support running commands in foreground and background processes

8.) Implement custom handlers for 2 signals, SIGINT and SIGTSTP

----------------------------------------------

PROVIDE A PROMPT FOR RUNNING COMMANDS:

----------------------------------------------

*Use the colon : symbol as a prompt for each command line. 

*The general syntax of a command line is:
"command [arg1 arg2 ...] [< input_file] [> output_file] [&]"
…where items in square brackets are optional.

*Assume that a command is made up of words separated by spaces.

*The special symbols <, > and & are recognized, but they must be surrounded by spaces like other words.

*If the command is to be executed in the background, the last word must be &. If the & character appears anywhere else, just treat it as normal text.

*If standard input or output is to be redirected, the > or < words followed by a filename word must appear after all the arguments. Input redirection can appear before or after output redirection.

*The shell does not need to support any quoting; so arguments with spaces inside them are not possible. We are also not implementing the pipe "|" operator.

*The shell must support command lines with a maximum length of 2048 characters, and a maximum of 512 arguments.

* do not need to do any error checking on the syntax of the command line.

----------------------------------------------

HANDLE BLANK LINES AND COMMENTS

----------------------------------------------

*The shell should allow blank lines and comments.

*Any line that begins with the # character is a comment line and should be ignored. Mid-line comments, such as the C-style //, will not be supported.

*A blank line (one without any commands) should also do nothing.

*The shell should just re-prompt for another command when it receives either a blank line or a comment line.

----------------------------------------------

PROVIDE EXPANSION FOR THE VARIABLE $$

----------------------------------------------

*The shell must expand any instance of "$$" in a command into the process ID of the smallsh itself. The shell does not otherwise perform variable expansion. 

----------------------------------------------

EXECUTE 3 COMMANDS VIA CODE BUILT INTO THE SHELL (exit, cd, status)

----------------------------------------------

*do not have to support input/output redirection for these built in commands

*These commands do not have to set any exit status.

*If the user tries to run one of these built-in commands in the background with the & option, ignore that option and run the command in the foreground anyway (i.e. don't display an error, just run the command in the foreground).

exit : exits the shell, takes no args, when run shell must kill any other processes or jobs that the shell has started before it terminates.

cd : changes working directory of shell, if no args changes to directory specified in the HOME enviroment variable, otherwise go to path specified (relative or absolute)

status : prints out either the exit status or the terminating signal of the last foreground process ran by the shell, if run before any foreground command then return exit status 0, should ignore built in commands

----------------------------------------------

