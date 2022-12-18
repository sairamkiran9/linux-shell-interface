# Project: 1
# Group: 9
# Members: SM22BT, SG22BX, MET19D
# Professor: Andy Wang, PhD
# Class: COP 4610
# Description: The makefile for project-1.

BINS = shell
C = gcc
CFLAGS = -std=gnu99 -Wall -pedantic -g

all: $(BINS)

#files included for shell execution
shell: main.c utils.c execute.c echo.c cd.c fileio.c pipe.c
	$(C) $(CFLAGS) -o shell main.c utils.c execute.c echo.c cd.c fileio.c pipe.c


#removes (cleans) executables
clean:
	rm $(BINS)
