
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                          //
//    COP4610   - Project-1 Group-9                                                                                         //
//    Members   - Sri Sai Ram Kiran Muppana, Matt Torres, Gongalla Shanmukh VishnuVardhan Rao                               //
//    Professor - Andy Wang, PhD                                                                                            //
//    Project   - Design and implement a basic shell interface that supports input/output redirection, piping, background   //
//    processing, and a series of built-in functions as specified below.                                                    //
//                                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @file main.c
 * @author Group - 9
 * @brief The main c file for executing the basic shell interface.
 *  This intitates the shell and starts taking the input from the user.
 *  Then input will be parsed into tokens based on the "|" delimiter and,
 *  space separated delimiter. Than it is passed as argument to execute the tokens.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "utils.h"

#define DEFAULT_BUFSIZE 5

char *get_input(void)
{
  /**
   * @brief This method fetches the input from the user until "\n" is entered
   *
   * @return user input in string format
   */
  int bufsize = 0;
  char *buffer = NULL;
  char cmd_input[DEFAULT_BUFSIZE];

  while (fgets(cmd_input, DEFAULT_BUFSIZE, stdin))
  {
    /*
     * fgets takes in the input in the size of DEFAULT_BUFSIZE and,
     * get concatinated to the buffer variable during each iteration.
     */
    char *eol = strchr(cmd_input, '\n');
    int cur_size = 0;

    if (eol != NULL)
    {
      cur_size = eol - cmd_input;
    }
    else
    {
      cur_size = DEFAULT_BUFSIZE - 1;
    }
    /*
     * reallocating the memorry size to add new input string.
     */
    buffer = (char *)realloc(buffer, bufsize + cur_size);
    memcpy(&buffer[bufsize], cmd_input, cur_size);
    bufsize += cur_size;

    if (eol != NULL)
    {
      /*
       * Break the loop if there is a new line entered by the user
       */
      break;
    }
  }

  buffer = (char *)realloc(buffer, bufsize + 1);
  buffer[bufsize] = 0;
  return buffer;
}

void execute_shell(tokenlist *tokens, tokenlist *args_list[])
{
  /**
   * @brief Check piping and execute arguments
   *  piping is checked based on the tokens size
   *  tokens size is valuated because we have parsed the tokens based on the "|" delimiter.
   *
   * @param pipe parsed tokens, space separated tokens
   * @return Nothing
   *
   */
  if (tokens->size == 2)
  {
    /*
     * single pipe tokens execution
     */
    execute_pipe(args_list[0], args_list[1]);
  }
  else if (tokens->size > 2)
  {
    /*
     * multi pipe execution, this method supports unlimited piping
     * the second argument is passed as "0" to indicate the method to
     * start executing piping from the start
     */
    multi_piping(args_list, tokens->size, 0);
  }
  else if (tokens->size == 1)
  {
    /*
     * For no pipe command execution the second argument is "0 "
     */
    execute_tokens(args_list[0], 0);
  }
}

int main()
{
  struct tm startTime = get_curtime(); // using the time object from the built-in module time
  int jobID = 0;
  while (1)
  {
    int background = 0;
    char *bg_str = NULL;
    print_prompt();
    char *input = get_input();
    tokenlist *tokens = get_tokens(input, "|");
    tokenlist *args_list[tokens->size];
    for (int i = 0; i < tokens->size; i++)
    {
      bg_str = strchr(tokens->items[i], '&');
      if ((bg_str != NULL) && (i == tokens->size - 1))
      {
        background = 1; // convert to 1 once the bg process works
      }
      args_list[i] = get_tokens(tokens->items[i], " ");
      args_list[i] = check_for_tilda(args_list[i]); // Replace the '~' char with $HOME value
    }

    if ((tokens->size >= 1) &&
        (args_list[0]->size != 0))
    {
      if (strcmp("exit", args_list[0]->items[0]) == 0)
      {
        /*
         * If user enters exit, than the shell breaks the loop, cleans the memory and exits.
         */
        clean_mem(input, tokens, args_list);
        break;
      }
      else if (strcmp("./shell", args_list[0]->items[0]) == 0)
      {
        /*
         * If user enters ./shell, than new shell is executed in the existing shell.
         */
        char *args[] = {"./shell", NULL};
        execv("./shell", args);
        printf("Unknown command\n"); // not specified if it should be in a child process?
      }
    }

    if (background == 1)
    {
      jobID += 1;
      int pid = fork();
      printf("[%d] %d\n", jobID, pid);
      int status = 0;
      if (pid == 0)
      {
        execute_shell(tokens, args_list);
        status = 1;
        jobID -= 1;
        printf("[%d] %s\n", jobID, input);
        exit(0);
      }
      else
      {
        waitpid(pid, &status, WNOHANG);
        background = 0;
      }
    }
    else
    {
      execute_shell(tokens, args_list);
    }
    clean_mem(input, tokens, args_list);
  }

  struct tm endTime = get_curtime();

  /*
   * Prints the total duration the user spent using the shell.
   */
  print_timePeriod(startTime, endTime);
  return 0;
}