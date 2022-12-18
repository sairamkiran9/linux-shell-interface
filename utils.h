#ifndef UTILS_H
#define UTILS_H

typedef struct
{
  int size;
  char **items;
} tokenlist;

typedef struct
{
  char *user;
  char *machine;
  char *pwd;
} prompt;

/**
 * @brief method creates a tokenlist object.
 *
 * @return intialised token object
 */
tokenlist *new_tokenlist(void);

/**
 * @brief Method to update "~" with $HOME value
 *  This method evalutes all the input arguments and replaces the "~" to $HOME
 *
 * @param tokenlist object tokens
 * @return tokens
 *
 */
tokenlist *check_for_tilda(tokenlist *tokens);

/**
 * @brief This is the parser method
 * This method parses the string input based on the provided delimiter.
 *
 * @param char input and delimter
 * @return token object that contains parsed strings data and number of arguments.
 */
tokenlist *get_tokens(char *input, char *delimiter);

/**
 * @brief This method appends each parsed string into an array of strings
 *
 * @return Nothing.
 */
void add_token(tokenlist *tokens, char *item);

/**
 * @brief Clean intialised objects
 *  This method uses free() to clean all the allocated bytes in the memory.
 *  The method runs a for loop over the tokenlist objects and free's each allocation.
 *
 * @param tokenlist object tokens to be cleaned
 */
void free_tokens(tokenlist *tokens);

/**
 * @brief This method changes the directory of the shell
 *
 * @return Nothing.
 */
void execute_cd(tokenlist *tokens);

/**
 * @brief each arguments.
 * If the argument starts with "$" than it checks for the variable value in environment.
 *
 * @return nothing
 */
void execute_echo(tokenlist *tokens);

/**
 * @brief file contains methods for IO redirection.
 * The current code supports the following formats.
 * - CMD > FILE
 * - CMD < FILE
 * - CMD < FILE_IN > FILE_OUT
 *
 * @return nothing
 */
void fileIO(int index, int flag, tokenlist *tokens);

/**
 * @brief deallocates tokens and input
 *
 * @return nothing
 */
void clean_mem(char *input, tokenlist *tokens, tokenlist *args_list[]);

/**
 * @brief Print prompt in the shell
 *  Each time this method is called a prompt in this format
 *  "USER@MACHINE : PWD >" will be printed in the shell.
 *  All the variables are fetched from the environment using the getenv() built-in fucntion.
 *
 * @return Nothing
 */
void print_prompt();

/**
 * @brief print tokens
 *  For the developer, to list down the list of arguments inputed by the user.
 *
 * @param tokenlist object tokens
 * @return Nothing
 */
void print_tokens(tokenlist *tokens);

/**
 * @brief Get the localtime using time module
 *
 * @return the current localtime back.
 *
 */
struct tm get_curtime(void);

/**
 * @brief Method to get total run time duration of the shell.
 *  This method takes two time objects and calculates the time difference between them.
 *
 * @param start time object and end time object
 * @return Nothing
 *
 */
void print_timePeriod(struct tm startTime, struct tm endTime);

/**
 * @brief Method to execute tokenlist of commands
 *
 * @param tokenlist *tokens, int pipe
 * @return Nothing
 *
 */
int execute_tokens(tokenlist *tokens, int pipe);

/**
 * @brief for single piping.
 * Two child process are created and executed according to the piping algorithm.
 *
 * @return
 */
void execute_pipe(tokenlist *cmd1, tokenlist *cmd2);

/**
 * @brief for multi-piping.
 * This is a recursive method that iterates and executes all pipes.
 * After the command args executed in the first child process,
 * in the second child process this method is called
 * again recursively to support multi piping.
 *
 * @return nothing
 */
void multi_piping(tokenlist *args_list[], int n, int index);

/**
 * @brief Executes Linux commands
 *  exit, echo, cd and jobs are implemented from scratch
 *  Remaining commands are executed with the help of execv() method and in-built binaries of Linux.
 *
 * @param pipe parsed tokens, space separated tokens,
 *  position of "<" or ">" characters, flag for IO redirection.
 * @return Nothing
 *
 */
void execute_builtin(tokenlist *tokens, tokenlist *args, int index, int flag);

#endif