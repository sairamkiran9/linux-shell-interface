/**
 * @file echo.c
 * @author your name (you@domain.com)
 * @brief This file executes the echo command functionality
 * Restriction: only space separated environment variables will be printed.
 * Examples:
 *  - echo $USER $HOME $PWD => works
 *  - echo kdflnskjf $USER  => works
 *  - echo $USER$HOME       => don't work
 *  - echo asdkjs$USER      => don't work
 *  - echo $USERadsad       => don't work
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

void get_envdata(char *var)
{
  /**
   * @brief the environment variable value if defined.
   *
   * @return nothing
   */
  char *variable = var + 1;
  char *value = getenv(variable);
  if (value != NULL)
    printf("%s ", value);
}

void execute_echo(tokenlist *tokens)
{
  /**
   * @brief Prints each arguments.
   *  If the argument starts with "$" than it checks for the variable value in environment.
   *
   * @return nothing
   */
  for (int i = 1; i <= tokens->size; i++)
  {
    if (i == tokens->size)
    {
      printf("\n");
    }
    else if (strncmp("$", tokens->items[i], 1) != 0)
    {
      printf("%s ", tokens->items[i]);
    }
    else
    {
      get_envdata(tokens->items[i]); // get environment value
    }
  }
}
