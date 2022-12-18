/**
 * @file execute.c
 * @author group-9
 * @brief This file contains the function to execute the shell commands.
 *   A child process will be created to execute the commands,
 *   except for the specified built-in methods.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.h"

int execute_tokens(tokenlist *tokens, int pipe)
{
  /**
   * @brief method that executes the Linux commands
   *
   * @param args to execute, pipe=1 if "|" present
   *
   */
  tokenlist *args = new_tokenlist();

  int i, flag = 0;
  for (i = 0; i < tokens->size; i++)
  {
    /*
     * check for IO redirection
     */
    if ((strcmp(tokens->items[i], ">") == 0) ||
        (strcmp(tokens->items[i], "<") == 0))
    {
      flag = 1;
      break;
    }
    add_token(args, tokens->items[i]);
  }

  if (args->size == 0)
  {
    /*
     * if input is NULL or empty cmd or newline
     */
    return 1;
  }
  else if (strcmp("exit", args->items[0]) == 0)
  {
    return 0;
  }
  else if (strcmp("./shell", args->items[0]) == 0)
  {
    /*
     * If user enters ./shell, than new shell is executed in the existing shell.
     */
    char *args[] = {"./shell", NULL};
    execv("./shell", args);
    printf("Unknown command\n"); // not specified if it should be in a child process?
  }
  else if ((pipe == 0) && (strcmp("cd", args->items[0]) == 0))
  {
    /*
     * This method call will execute the cd command functionality.
     */
    execute_cd(args);
  }
  else if (pipe == 1)
  {
    /*
     * This method will execute the built-in funcitons other than cd, echo, exit and jobs
     */
    execute_builtin(tokens, args, i, flag);
  }
  else
  {
    int pid = fork();
    int status = 0;
    if (pid == 0)
    {
      status = 1;
      execute_builtin(tokens, args, i, flag);
      exit(0);
    }
    else
    {
      waitpid(pid, &status, 0);
    }
  }
  free_tokens(args);
  return 1;
}