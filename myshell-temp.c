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
  int pipes = 0;

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
    int number_of_args;

    for (int i = 0; input[i] != NULL; i++) {
      number_of_args++;
    }
    // printf("%d\n")
    char * filtered_cmd[number_of_args];
    printf("array initialized with size %d\n", number_of_args);

    /* loop through args */
    for (int i = 0; input[i] != NULL; ++i) {

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

          /*for (int x = i; input[x] == NULL; x++) {
          	filtered_cmd[x] = input[x];
          }*/
          input[i] = NULL;
          input[i + 1] = NULL;
          /* 1 for stdout, 2 for stderr */
          dup2(writeFile, 1);
          /*dup2(writeFile, 2);*/
          close(writeFile);
          /* execute the program */
          execvp(input[0], input);
          exit(1);
        }
        /* forking error */
        else if (pid == -1) {
          perror("fork");
          exit(1);
        }
      }

      /* input redirection */
      if (strcmp(input[i], "<") == 0) {
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

          /*for (int x = i; input[x] == NULL; x++) {
          	filtered_cmd[x] = input[x];
          }*/
          input[i] = NULL;
          input[i + 1] = NULL;
          /* 0 for stdin */
          dup2(readFile, 0);
          close(readFile);
          /* execute the program */
          execvp(input[0], input);
          exit(1);
        }
        /* forking error */
        else if (pid == -1) {
          perror("fork");
          exit(1);
        }
      }

      /* piping - first process */
      if (strcmp(input[i], "|") == 0) {
        printf("i found a pipe");
        first_cycle = 1;
        already_executed = 1;

        int pid = fork();
      	/* parent process: shell */
      	if (pid > 0) {
      	   wait(NULL);
      	}
      	/* child process */
      	else if (pid == 0) {

          dup2(pipefd[1], 1);
          close(pipefd[0]);
          close(pipefd[1]);
        	/* execute the program */
          execlp(input[i-1], input[i-1], "-l", NULL);
        	//execvp(input[i-1], input);
        	exit(1);
      	}
      	/* forking error */
      	else if (pid == -1) {
        	perror("fork");
       	  exit(1);
      	}
      }

      /* piping - second process */
      if (first_cycle == 1) {
        if (strcmp(input[i-1], "|") == 0) {
          printf("program after pipe");
          already_executed = 1;
          int pid = fork();
          /* parent process */
          if (pid > 0) {
            wait(NULL);
          }
          /* child process */
          else if (pid == 0) {
            dup2(pipefd[0], 0);
            close(pipefd[0]);
            /* execute the program */
            execvp(input[i], input);
            //execlp(input[i], input[i], "output.txt", NULL);
            exit(1);
          }
          /* forking error */
          else if (pid == -1) {
            perror("fork");
            exit(1);
          }
        }
      }
      if ( !(strcmp(input[i], "|")) && !(strcmp(input[i], ">")) && !(strcmp(input[i], "<")) ) {
        filtered_cmd[i] = input[i];
      }
      //printf("Item %i of input: %s\n", i, input[i]);
    }

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

    for (int i = 0; i < number_of_args; i++) {
      printf("%s",filtered_cmd[i]);
    }

  }

  return 0;
}
