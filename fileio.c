/**
 * @file fileio.c
 * @author Group - 9
 * @brief This file contains methods for IO redirection.
 * The current code supports the following formats.
 * - CMD > FILE
 * - CMD < FILE
 * - CMD < FILE_IN > FILE_OUT
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "utils.h"

void outputIO(char *outFile)
{
    /**
     * @brief method is called when ">" character is present in the input arguments.
     * It will redirect the STDOUT to the specified file.
     *
     * @return nothing
     */
    int outFD = open(outFile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    close(1);
    dup2(outFD, 1); // stdout
    close(outFD);
}

void inputIO(char *inFile)
{
    /**
     * @brief method is called when "<" character is present in the input arguments.
     * It will redirect the specified file content to STDIN.
     */
    if (access(inFile, F_OK))
    {
        /*
         * To check if the file or directory exists or not
         */
        perror(inFile);
        exit(0);
    }
    else
    {
        int inFD = open(inFile, O_RDONLY, 0644);
        close(0);
        dup2(inFD, 0); // stdin
        close(inFD);
    }
}

void fileIO(int index, int flag, tokenlist *tokens)
{
    /*
     * This is the main IO redirection method that handles errors and the IO redirection.
     */
    if (index + 1 < tokens->size)
    {
        if ((strcmp(tokens->items[index], ">") == 0) &&
            (tokens->items[index + 1] != NULL))
        {
            outputIO(tokens->items[index + 1]);
        }
        else if ((strcmp(tokens->items[index], "<") == 0) &&
                 (tokens->items[index + 1] != NULL))
        {
            inputIO(tokens->items[index + 1]);
        }
        else
        {
            printf("syntax error near unexpected token 'newline'\n");
            exit(0);
        }
    }
    else if (flag == 1)
    {
        printf("syntax error near unexpected token 'newline'\n");
        exit(0);
    }
}