# Project-1

### Implementation of basic shell interface

- <b>Course</b> &nbsp;&nbsp;&nbsp; : COP 4610 Operating Systems
- <b>Professor</b>  : Dr. Andy Wang
- <b>Group</b>  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; : 9
- <b>Group Members</b>  :
    - SM22BT &nbsp;: Sri Sai Ram Kiran Muppana 
    - SG22BX &nbsp;: Shanmukh Vishnuvardhan Rao Gongalla
    - MET19D &nbsp;: Matthew Torres

### Divison of Labor

| NAME     | PART       |
|------    |----------- |
| Muppana  |2-4, 6-10   |
| Gongalla |4, 6, 7     |
| Torres   |5-7, 9      |


#### List of Files

```
Makefile  :   The make file of this project for executing the developed shell interface.

C files
=======
cd.c      :   This file implements the cd command functionality.
              Throws errors when there is an error or an invalid argument passed by the user.

echo.c    :   This file executes the echo command functionality.

execute.c :   This file contains the function to execute the shell commands.
              A child process will be created to execute the commands.
              Except for the specified built-in methods.

fileio.c  :   This file contains methods for input and output redirection to
              support '>' and '<' as arguments.

pipe.c    :   This file has methods that support single piping and multi-piping

main.c    :   This is the main c file for executing the basic shell interface.
              This intitates the shell and starts taking the input from the user.
              Then input will be parsed into tokens based on space separated delimited and,
              passed as argument to execute the tokens.

utlis.c   :   This file contains common functions that are used by other c files.

Header files
============

utlis.h
```

### How to run the file

call ```make```

call ```./shell```

Now Basic shell is open up and you can execute commands among given parts

### Sample Test cases

call ```echo $USER $HOME```

call ```echo $PWD > file```

call ```cat file```

call ```cat < file```

call ```cat < file > file2```

call ```cat file2```

create 4 rows of string data in file3

check data with ```cat file3```

call  ```cat file3 | sort```

call ```cat file3 | sort | grep s```

call ```cat file3 | sort | grep s | sort -r```

call ```cat file3 | sort | grep s | sort -r > file4```

call ```cat file4```

call ```./shell```

call ```echo ~```

call ```cd ~```

call ```cd -```



#### Known Bugs
```
- "cd ~/path" has a bug where it unexpectedly updates $HOME.
- background process works not implemented completely, i.e the the job numbers are not printed correctly.
- background process has memory leaks.
- haven't implemented builtin function "jobs".
```

### Extra Credits
- Support unlimited number of pipes.[2]
- Support piping and I/O redirection in a single command.[2]
- Shell-ception: can execute your shell from within a running shell process repeatedly [1]  
- Support piping and IO redirection in the built-in programs.[2]
