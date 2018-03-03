#include <stdlib.h>
#include <stdio.h>

char *cmd1[] = { "/bin/ls", "-al", "/", 0 };
char *cmd2[] = { "/usr/bin/tr", "a-z", "A-Z", 0 };


int main(int argc, char **argv) {
	int pid, status;
	int fd[2];

	pipe(fd);

	int pid_1;
  pid_1 = fork();

	if (pid_1 == 0) {
		dup2(fd[1], 1);	/* this end of the pipe becomes the standard output */
		close(fd[0]); 		/* this process don't need the other end */
		execvp(cmd1[0], cmd1);	/* run the command */
		perror(cmd1[0]);	/* it failed! */
	}
	else if (pid_1 == -1) {
		perror("fork");
		exit(1);
	}

	int pid_2;
  pid_2 = fork();

	if (pid_2 == 0) {
		dup2(fd[0], 0);	/* this end of the pipe becomes the standard input */
		close(fd[1]);		/* this process doesn't need the other end */
		execvp(cmd2[0], cmd2);	/* run the command */
		perror(cmd2[0]);	/* it failed! */
	}
	else if (pid_2 == -1) {
		perror("fork");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]); 	/* this is important! close both file descriptors on the pipe */

	while (wait(NULL) != -1)	/* pick up all the dead children */
	exit(0);
}
