/**
 * @file utils.c
 * @author group-9
 * @brief Contains common functions that are used by other c files of the project
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "utils.h"

tokenlist *new_tokenlist(void)
{
  tokenlist *tokens = (tokenlist *)malloc(sizeof(tokenlist));
  tokens->size = 0;
  tokens->items = (char **)malloc(sizeof(char *));
  tokens->items[0] = NULL; /* make NULL terminated */
  return tokens;
}

void add_token(tokenlist *tokens, char *item)
{
  int i = tokens->size;
  tokens->items = (char **)realloc(tokens->items, (i + 2) * sizeof(char *));
  tokens->items[i] = (char *)malloc(strlen(item) + 1);
  tokens->items[i + 1] = NULL;
  if (strcmp(item, "&") != 0)
  {
    strcpy(tokens->items[i], item);
    tokens->size += 1;
  }
}

tokenlist *get_tokens(char *input, char *delimiter)
{
  char *buf = (char *)malloc(strlen(input) + 1);
  strcpy(buf, input);

  tokenlist *tokens = new_tokenlist();

  char *tok = strtok(buf, delimiter);
  while (tok != NULL)
  {
    add_token(tokens, tok);
    tok = strtok(NULL, delimiter);
  }
  free(buf);
  return tokens;
}

void free_tokens(tokenlist *tokens)
{
  for (int i = 0; i < tokens->size; i++)
  {
    free(tokens->items[i]);
  }
  free(tokens->items);
  free(tokens);
}

void print_prompt(void)
{
  prompt p;
  p.user = getenv("USER");
  p.machine = getenv("MACHINE");
  p.pwd = getenv("CWD");
  if (p.pwd == NULL)
  {
    p.pwd = getenv("PWD");
  }
  printf("%s@%s:%s>", p.user, p.machine, p.pwd);
}

void print_tokens(tokenlist *tokens)
{
  for (int i = 0; i < tokens->size; i++)
  {
    printf("token %d: (%s) \n", i, tokens->items[i]);
  }
}

tokenlist *check_for_tilda(tokenlist *tokens)
{
  for (int i = 0; i < tokens->size; i++)
  {
    char *path = getenv("HOME");
    char *tilda_str = NULL;
    tilda_str = strchr(tokens->items[i], '~');
    if (tilda_str != NULL)
    {
      tilda_str = tilda_str + 1;
      strcat(path, tilda_str);
      tokens->items[i] = (char *)realloc(tokens->items[i], strlen(path) + 1);
      memcpy(tokens->items[i], path, strlen(path) + 1);
    }
  }
  return tokens;
}

struct tm get_curtime(void)
{
  time_t t1 = time(NULL);
  struct tm tm = *localtime(&t1);
  return tm;
}

void print_timePeriod(struct tm startTime, struct tm endTime)
{
  if (startTime.tm_sec > endTime.tm_sec)
  {
    endTime.tm_sec += 60;
    startTime.tm_min -= 1;
  }
  if (startTime.tm_min > endTime.tm_min)
  {
    endTime.tm_min += 60;
    startTime.tm_hour -= 1;
  }
  printf("time elapsed: %02d hours, %02d minutes, and %02d seconds\n",
         endTime.tm_hour - startTime.tm_hour,
         endTime.tm_min - startTime.tm_min,
         endTime.tm_sec - startTime.tm_sec);
}

void execute_builtin(tokenlist *tokens, tokenlist *args, int index, int flag)
{
  for (int k = index; k < tokens->size; k++)
  {
    if ((strcmp(tokens->items[k], ">") == 0) || (strcmp(tokens->items[k], "<") == 0))
    {
      flag = 1;
      fileIO(k, flag, tokens); /* Call IO redirection method */
    }
    flag = 0;
  }
  if (strcmp("echo", args->items[0]) == 0)
  {
    execute_echo(args); /* This method call will execute the echo command functionality. */
  }
  else
  {
    char *path = getenv("PATH");
    tokenlist *directories = get_tokens(path, ":");
    for (int j = 0; j < directories->size - 1; j++)
    {
      directories->items[j] = (char *)realloc(directories->items[j],
                                              (strlen(directories->items[j]) +
                                               strlen(args->items[0]) + 2));
      strcat(directories->items[j], "/");
      strcat(directories->items[j], args->items[0]);
      execv(directories->items[j], args->items);
    }
    printf("Unknown command\n");
    free_tokens(directories);
  }
}

void clean_mem(char *input, tokenlist *tokens, tokenlist *args_list[])
{
  for (int i = 0; i < tokens->size; i++)
  {
    free_tokens(args_list[i]);
  }
  free(input);
  free_tokens(tokens);
}
