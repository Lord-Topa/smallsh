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


PROVIDE A PROMPT FOR RUNNING COMMANDS:

*Use the colon : symbol as a prompt for each command line. 

*The general syntax of a command line is:

"command [arg1 arg2 ...] [< input_file] [> output_file] [&]"

…where items in square brackets are optional.

*You can assume that a command is made up of words separated by spaces.

*The special symbols <, > and & are recognized, but they must be surrounded by spaces like other words.

*If the command is to be executed in the background, the last word must be &. If the & character appears anywhere else, just treat it as normal text.

*If standard input or output is to be redirected, the > or < words followed by a filename word must appear after all the arguments. Input redirection can appear before or after output redirection.

*Your shell does not need to support any quoting; so arguments with spaces inside them are not possible. We are also not implementing the pipe "|" operator.

*Your shell must support command lines with a maximum length of 2048 characters, and a maximum of 512 arguments.

*You do not need to do any error checking on the syntax of the command line.
