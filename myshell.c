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

    int first_cycle = 0;
    int first_pipe = 0;
    int pid, status;
    int pipe_A[2];

    pipe(pipe_A);
    int number_of_args = 0;
    int current_arg = 0;
    int output_already_redirected = 0;

    /* create temporary array */
    for (int i = 0; input[i] != NULL; i++) {
      if (i == 0) {
        number_of_args++;
      } else if ( (strcmp(input[i-1], ">") == 0) || (strcmp(input[i-1], "<") == 0) ) {
          //number_of_args--;
      } else if ( (strcmp(input[i], "|") != 0) && (strcmp(input[i], ">") != 0) && (strcmp(input[i], "<")!= 0) ) {
          number_of_args++;
      }
    }
    number_of_args++;
    char * filtered_cmd[number_of_args];
    printf("array initialized with size %d\n", number_of_args);

    /* replace garbage with NULL */
    for (int i = 0; i <= number_of_args; i++) {
         filtered_cmd[i] = "NULL";
    }

    /* loop through args */
    for (int i = 0; input[i] != NULL; ++i) {

      /* make filtered_cmd */
      if (first_cycle == 1) {
        if ( (strcmp(input[i-1], ">") == 0) || (strcmp(input[i-1], "<") == 0) ) {
          continue;
        } else if ( (strcmp(input[i], "|") != 0) && (strcmp(input[i], ">") != 0) && (strcmp(input[i], "<")!= 0) ) {
          filtered_cmd[current_arg] = input[i];
          current_arg++;
        }
      } else if (first_cycle == 0) {
        if ( (strcmp(input[i], "|") != 0) && (strcmp(input[i], ">") != 0) && (strcmp(input[i], "<")!= 0) ) {
          filtered_cmd[current_arg] = input[i];
          current_arg++;
        }
      }

      /* exit function */
      if (strcmp(input[0], "exit") == 0) {
        exit(-1);
      }

      /* cd function */
      if (strcmp(input[0], "cd") == 0) {
        if (argc == 1) {
          chdir("/");
        }
        sprintf(relative_path, "%s/%s", cwd, input[1]);
        chdir(relative_path);
      }

      /* input redirection */
      if (strcmp(input[i], "<") == 0) {
        printf("found <\n");
        output_already_redirected = 1;
        first_cycle = 1;

        /* put NULL to execute command */
        for (int i = 0; i < number_of_args; i++) {
          if (strcmp(filtered_cmd[i], "NULL") == 0) {
            filtered_cmd[i] = NULL;
            break;
          }
        }

        int pid = fork();
        /* parent process */
        if (pid > 0) {
          wait(NULL);
        }
        /* child process */
        else if (pid == 0) {
          readFile = open(input[i + 1], O_RDONLY);

          /* 0 for stdin */
          dup2(readFile, 0);
          close(readFile);
          /* execute the program */
          execvp(filtered_cmd[0], filtered_cmd);
          exit(1);
        }
        /* forking error */
        else if (pid == -1) {
          perror("fork");
          exit(1);
        }
        /* empty the array */
        for (int i = 0; i <= number_of_args; i++) {
             filtered_cmd[i] = "NULL";
        }
        current_arg = 0;
      }

      /* output redirection */
      if (strcmp(input[i], ">") == 0) {
        first_cycle = 1;
        output_already_redirected = 1;

        /* put NULL to execute command */
        for (int i = 0; i < number_of_args; i++) {
          if (strcmp(filtered_cmd[i], "NULL") == 0) {
            filtered_cmd[i] = NULL;
            break;
          }
        }

        int pid = fork();
        /* parent process */
        if (pid > 0) {
          wait(NULL);
        }
        /* child process */
        else if (pid == 0) {
          writeFile = open(input[i + 1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

          /* 1 for stdout, 2 for stderr */
          dup2(writeFile, 1);
          /*dup2(writeFile, 2);*/
          close(writeFile);
          /* execute the program */
          execvp(filtered_cmd[0], filtered_cmd);
          exit(1);
        }
        /* forking error */
        else if (pid == -1) {
          perror("fork");
          exit(1);
        }
        /* empty the array */
        current_arg = 0;
        /* incase pipes */
        close(pipe_A[0]);
        close(pipe_A[1]);
      }

      /* piping */
      if (strcmp(input[i], "|") == 0) {
        printf("i found a pipe\n");
        first_cycle = 1;

        /* put NULL to execute command */
        for (int i = 0; i < number_of_args; i++) {
          if (strcmp(filtered_cmd[i], "NULL") == 0) {
            filtered_cmd[i] = NULL;
            break;
          }
        }

        int PID_1 = fork();
      	/* parent process: shell */
      	if (PID_1 > 0) {
      	   wait(NULL);
      	}
      	/* child process */
      	else if (PID_1 == 0) {

          /* redirect standard output to pipe_A write end */
          dup2(pipe_A[1], 1);
          close(pipe_A[0]);

        	/* execute the program */
        	execvp(filtered_cmd[0], filtered_cmd);
          exit(1);
      	}
      	/* forking error */
      	else if (PID_1 == -1) {
        	perror("Can't fork");
       	  exit(1);
      	}
        /* show me the array up to this moment */
        for (int i = 0; i < 4; i++) {
            printf("Item %i of filtered_cmd: %s\n", i, filtered_cmd[i]);
        }
        /* empty the array */
        for (int i = 0; i <= number_of_args; i++) {
             filtered_cmd[i] = "NULL";
        }
        current_arg = 0;
      }

    }
    /* end of loop */

    /* execute the last command */
    /* only if the last output is not redirected */
    if (output_already_redirected == 0) {
      printf("ana d5lt\n");
      int PID_4 = fork();
      /* parent process: shell */
      if (PID_4 > 0) {
         wait(NULL);
      }
      /* child process */
      else if (PID_4 == 0) {
        printf("I am the last command\n");
        /* redirect standard input to pipe_A read end */
        dup2(pipe_A[0], 0);
        close(pipe_A[1]);

        /* put NULL to execute command */
        for (int i = 0; i < number_of_args; i++) {
          if (strcmp(filtered_cmd[i], "NULL") == 0) {
            filtered_cmd[i] = NULL;
            break;
          }
        }
        /* show me the array up to this moment */
        for (int i = 0; i < 4; i++) {
            printf("Item %i of filtered_cmd: %s\n", i, filtered_cmd[i]);
        }
        printf("will execute the last command\n");
        execvp(filtered_cmd[0], filtered_cmd);
      	exit(0);
      }
      /* forking error */
      else if (PID_4 == -1) {
        perror("Can't fork");
        exit(1);
      }
      close(pipe_A[0]);
      close(pipe_A[1]);
    }

  }
  return 0;
}
