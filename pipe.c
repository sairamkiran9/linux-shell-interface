/**
 * @file pipe.c
 * @author group - 9
 * @brief This file has methods that support single piping and multi-piping
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.h"

void execute_pipe(tokenlist *cmd1, tokenlist *cmd2)
{
    /**
     * @brief Method for single piping.
     * Two child process are created and executed according to the piping algorithm.
     */
    int p_fds[2]; // file descriptor array for piping
    if (pipe(p_fds) == -1)
    {
        perror("pipe");
        exit(0);
    }
    int pid1 = fork(); // child process 1
    int status1 = 0;
    if (pid1 == 0)
    {
        close(p_fds[0]);
        dup2(p_fds[1], 1);       // output redirection
        execute_tokens(cmd1, 1); // execute commands for 1st args set
        status1 = 1;
        exit(1);
    }
    int pid2 = fork(); // child process 2
    int status2 = 0;
    if (pid2 == 0)
    {
        close(p_fds[1]);
        dup2(p_fds[0], 0);       // input redirection
        execute_tokens(cmd2, 1); // execute commands for 1st args set
        status2 = 1;
        exit(1);
    }
    close(p_fds[0]);
    close(p_fds[1]);
    waitpid(pid1, &status1, 0); // wait for child process 1 to complete
    waitpid(pid2, &status2, 0); // wait for child process 1 to complete
}

void multi_piping(tokenlist *args_list[], int n, int index)
{
    /**
     *@brief Method for multi-piping.
     * This is a recursive method that iterates and executes all pipes.
     * After the command args executed in the first child process,
     * in the second child process this method is called
     * again recursively to support multi piping.
     */
    if (index < n - 1)
    {
        int p_fds[2]; // file descriptor array for piping
        if (pipe(p_fds) == -1)
        {
            perror("pipe ");
            exit(0);
        }
        int pid1 = fork(); // child process 1
        int status1 = 0;
        if (pid1 == 0)
        {
            close(p_fds[0]);
            dup2(p_fds[1], 1);                   // output redirection
            execute_tokens(args_list[index], 1); // execute commands for 1st args set
            status1 = 1;
            exit(1);
        }
        int pid2 = fork(); // child process 2
        int status2 = 0;
        if (pid2 == 0)
        {
            close(p_fds[1]);
            dup2(p_fds[0], 0);                     // input redirection
            multi_piping(args_list, n, index + 1); // recursive call
            status2 = 1;
            exit(1);
        }
        close(p_fds[0]);
        close(p_fds[1]);
        waitpid(pid1, &status1, 0); // wait for child process 1 to complete
        waitpid(pid2, &status2, 0); // wait for child process 2 to complete
    }
    else
    {
        execute_pipe(args_list[index - 1], args_list[index]); // For the last pipe
    }
}