#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

extern char * * getline1();
char * getcwd(char * buf, size_t size);
char cwd[1024];
char relative_path[1024];

int main(int argc, char * argv[]) {
  int i;
  char * * input;
  int writeFile;
  int readFile;

  while (1) {

    /* get current working directory and prompt */
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("You@My_Shell:%s # ", cwd);
    } else {
      printf("\nMy Shell>> ");
    }

    input = getline1();

    int already_executed = 0;
    int first_cycle = 0;
    int first_pipe = 0;
    int pipefd[2];

    /* exit function */
    if (strcmp(input[0], "exit") == 0) {
      exit(-1);
    }

    int pid = fork();
    /* parent process */
    if (pid > 0) {
      wait(NULL);
    }
    /* child process */
    else if (pid == 0) {
      for (int i = 0; input[i] != NULL; ++i) {

        /* cd function */
        if (strcmp(input[0], "cd") == 0) {
          if (argc == 1) {
            chdir("/");
          }
          sprintf(relative_path, "%s/%s", cwd, input[1]);
          chdir(relative_path);
        }

        /* output redirection */
        else if (strcmp(input[i], ">") == 0) {
          printf("found output redirection\n");
          first_cycle = 1;
          already_executed = 1;
          writeFile = open(input[i + 1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
          input[i] = NULL;
          input[i + 1] = NULL;

          /* 1 for stdout, 2 for stderr */
          dup2(writeFile, 1);
          /*dup2(writeFile, 2);*/
          close(writeFile);
        }

        /* input redirection */
        else if (strcmp(input[i], "<") == 0) {
          printf("found output redirection\n");
          first_cycle = 1;
          already_executed = 1;
          /* child process */
          readFile = open(input[i + 1], O_RDONLY);
          input[i] = NULL;
          input[i + 1] = NULL;

          /* 0 for stdin */
          dup2(readFile, 0);
          close(readFile);
        }

      }
      execvp(input[i], input);
      exit(1);
    }
    /* forking error */
    else if (pid == -1) {
      perror("fork");
      exit(1);
    }

    /* loop through args */
    for (int i = 0; input[i] != NULL; ++i) {
      printf("Item %i of input: %s\n", i, input[i]);
    }

  }

  return 0;
}
