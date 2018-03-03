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
    int pipe_A[2];

    pipe(pipe_A);
    int number_of_args = 0;
    int current_arg = 0;

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
      filtered_cmd[number_of_args-1] = NULL;

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

      /* output redirection */
      if (strcmp(input[i], ">") == 0) {
        printf("found >");
        first_cycle = 1;
        already_executed = 1;
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
      }

      /* input redirection */
      if (strcmp(input[i], "<") == 0) {
        printf("found <");
        first_cycle = 1;
        already_executed = 1;
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
        current_arg = 0;
      }

      /* piping - first process */
      if (strcmp(input[i], "|") == 0) {
        printf("i found a pipe\n");
        first_cycle = 1;
        already_executed = 1;

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
          close(pipe_A[1]);

        	/* execute the program */
        	execvp(filtered_cmd[i], filtered_cmd);
          printf("exectued first program\n");
        	exit(1);
      	}
      	/* forking error */
      	else if (PID_1 == -1) {
        	perror("Can't fork");
       	  exit(1);
      	}
        /* show me the array up to this moment */
        for (int i = 0; i < 2; i++) {
            printf("Item %i of filtered_cmd: %s\n", i, filtered_cmd[i]);
        }
        /* empty the array */
        for (int i = 0; i < number_of_args; i++) {
            filtered_cmd[i] == NULL;
        }
        current_arg = 0;
      }

      /* piping - second process */
      if (first_cycle == 1) {
        if (strcmp(input[i-1], "|") == 0) {
          printf("program after pipe\n");
          already_executed = 1;
          int PID_2 = fork();
          /* parent process */
          if (PID_2 > 0) {
            close(pipe_A[0]);
            close(pipe_A[1]);
            wait(NULL);
          }
          /* child process */
          else if (PID_2 == 0) {

            /* redirect standard input to pipe_A read end */
            dup2(pipe_A[0], 0);
            close(pipe_A[1]);
            close(pipe_A[0]);

            /* execute the program */
            //printf("will execute now-after pipe\n");
            execvp(filtered_cmd[i], filtered_cmd);
            exit(1);
          }
          /* forking error */
          else if (PID_2 == -1) {
            perror("fork");
            exit(1);
          }
        }
      }
    }

    /* show me the array up to this moment */
    for (int i = 0; i < 2; i++) {
        printf("Item %i of filtered_cmd: %s\n", i, filtered_cmd[i]);
    }
    /* empty the array */
    for (int i = 0; i < number_of_args; i++) {
        filtered_cmd[i] == NULL;
    }
    current_arg = 0;


    if (already_executed == 0) {
    	int pid = fork();
    	/* parent process: shell */
    	if (pid > 0) {
    	   wait(NULL);
    	}
    	/* child process - first */
    	else if (pid == 0) {
      	   /* execute the program */
      	   execvp(input[0], input);
      	   exit(1);
    	}
    	/* handle forking error */
    	else if (pid == -1) {
      	   perror("fork");
     	   exit(1);
    	}
    }

  }

  return 0;
}
