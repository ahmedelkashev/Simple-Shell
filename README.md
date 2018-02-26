# Simple Shell Implementation in C
Implementing a FreeBSD shell program.
This basic shell is the first assignment for ECEN407-Operating Systems course. The target behind this FreeBSD shell is to utilize system calls like `exit()`, `chdir()`, `fork()`, `execvp()` and many more to perform basic shell operations and use it to run other programs.
The program is completely written in `C`, and uses the default compiler for `FreeBSD` which uses the command `cc` to get compiled.

## How to start the shell

- Clone or download this repository.
- Navigate to the directory and open it on terminal.
- Compile the source code using `make`.
- Now you have an executable with the name of `myshell`.
- Run the shell using the command `./myshell`.
- You can remove the executables again using `make clear`.

What does this Shell support?
-----------------------------
- Single commands with no arguments, example: `exit`, `ls`, `ps`.
- Single commands with arguments and flags: `ls -l`, `ps aux`, `cd ..`.
- Redirecting input & output: `cat input.txt > output.txt`
- "To be implemented", Piping commands: `cat input.txt | grep 2`

How does this Shell work?
-------------------------
This shell utilizes a lexical analyser to read and parse the input line and uses the function `getline()` to provide an array of pointers to character strings. Each string is either a word containing the letters, numbers, period (.), and forward slash (/), or a character string containing one of the special characters: (, ), <, >, |, &, or ;.
The shell then uses the input array to determine which command was written by the user using the `strcmp()` function and decides which function should it call from the `/bin/` directory.
If the input string contains any of the character strings like `<`, `>`, `|`, it behaves accordingly.

Note
----
In any operating system, the shell looks for the commands in the PATH. however, in this shell, it can only look for programs inside the `/bin/` directory. this will be updated soon.

System Calls
------------
- `getcwd()`: The [getcwd()](http://man7.org/linux/man-pages/man2/getcwd.2.html) system call gets current working directory.
- `fork()`: The [fork()](http://man7.org/linux/man-pages/man2/fork.2.html) system call creates a new process by duplicating the calling process. The new process is referred to as the child process. The calling process is referred to as the parent process.
- `wait()`: The [wait()](http://man7.org/linux/man-pages/man2/waitpid.2.html) system call suspends execution of the calling process until one of its children terminates.
- `dup2()`: The [dup2()](http://man7.org/linux/man-pages/man2/dup.2.html) system call creates a copy of the file descriptor, using the the file descriptor number specified in its parameters. and helps performing redirecting the input & output to the specified target: 0 for `STDIN`, 1 for `STDOUT`, 2 for `STDERR`.
Example: `dup2(a,b)`: makes b connected to what a is connected to. `dup2(writeFile, 1)` makes 1-stdout conntected to whichever writeFile specified.
- `execvp()`: The [execvp()](http://man7.org/linux/man-pages/man3/exec.3.html) system call belongs to the `exec` family, but with its `vp` in the end, it specifies that it takes a vector, and it expects you to be in the right path in order to operate correctly.
- `open()`: The [open()](http://man7.org/linux/man-pages/man2/open.2.html) system call opens the file specified by file name. and takes a second argument of _flags_.
- `close()`: The [close()](http://man7.org/linux/man-pages/man2/close.2.html) system call closes the file after finishing I/O operation
- `read()`: [read()](http://man7.org/linux/man-pages/man2/read.2.html) reads up to an x number of bytes from a certain file into the buffer. A buffer has to be created first.
- `write()`: [write()](http://man7.org/linux/man-pages/man2/write.2.html) writes up to an x number bytes from the buffer starting at a certain point to the file referred to in the function declaration.
- `perror()`: [perror()](http://man7.org/linux/man-pages/man3/perror.3.html) prints a system error message.

Flags
-----
- `O_RDONLY`: short for Read Only.
- `O_RDWR`: Open for reading and writing.
- `O_CREAT`: creates the file if it is not existent.
- `S_IRUSR`: read permission, owner. The "S" is for STAT, the "I" for INODE, the "R" for READ and the "USR" for USER.
- `S_IWUSR`: write permission, owner. The "S" is for STAT, the "I" for INODE, the "W" for WRITE and the "USR" for USER.


Assignment By: Ahmed SalahEldin Farouk Elkashef - 1410216.
