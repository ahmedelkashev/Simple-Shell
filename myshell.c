#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


extern char **getline();
char *getcwd(char *buf, size_t size);
char cwd[1024];
char relative_path[1024];
const int PIPE_READ = 0;
const int PIPE_WRITE = 1;
/* easier to use pipeEnds[PIPE_READ] and pipeEnds[PIPE_WRITE] */

int main(int argc, char *argv[]) {
  int i;
  char **input;
  char ** filtered_cmd[argc];

  char buffer[1024];
  int writeFile;
  int readFile;
  int pipes = 0;

  while (1) {
    
    /* get current working directory and prompt */
    if ( getcwd(cwd, sizeof(cwd) ) != NULL) {
        printf("You@My_Shell:%s # ", cwd);
    } else {
	printf("\nMy Shell>> ");
    }

    input = getline();

    /* if there are arguments */
    if (argc > 0) {
	
	int pid = fork();

	/* parent process: shell */
	if (pid > 0) {
	    wait(NULL);
	}
	/* child process - first */
	else if (pid == 0){

   	    for (int i = 0; input[i] != NULL; ++i) {
	    	/* redirect output to file */
		if( strcmp(input[i],">") == 0 ) {
			writeFile = open(input[i+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			
			/* reorganize the array
			for (int x = i; input[x] != NULL; x++) {
				if (input[x+2] == NULL) {
					input[x] = NULL;
					break;
				}
				input[x] = input[x+2];
			} */

			input[i] = NULL;
			input[i+1] = NULL;
			
			/* 1 for stdout, 2 for stderr */ 
			dup2(writeFile, 1);
			
			/*dup2(writeFile, 2);*/
			close(writeFile);
		}
		/* redirect input into process */
		else if ( strcmp(input[i],"<") == 0 ) {
			readFile = open(input[i+1], O_RDONLY);

			/* reorganize the array
			for (int x = i; input[x] != NULL; x++) {
				if (input[x+2] == NULL) {
					input[x] = NULL;
					break;
				}
				input[x] = input[x+2];
			} */
			
			input[i] = NULL;
			input[i+1] = NULL;
			
			/* 0 for stdin */
			dup2(readFile, 0);
			close(readFile);
		}
	    }

	    /* execute the program */
	    execvp(input[0],input);
	    exit(1);
	}
	/* handle forking error */
	else if (pid == -1) {
		perror("fork");
		exit(1);
	}
    }

    /* my implementation of cd and exit commands */
    for (int i = 0; input[i] != NULL; ++i) {


      /* exit the program */
      if ( strcmp(input[0],"exit") == 0 ) {
	exit(-1);
      }

      /* changing current directory */
      if ( strcmp(input[0],"cd") == 0 ) {
	
	/* if no argument for cd */
	if (argc == 1) {
	    chdir("/");
	}
	sprintf(relative_path,"%s/%s",cwd,input[1]);
	chdir(relative_path);
      }

      printf("Item %i of input: %s\n", i, input[i]);
    }
  }
  
  return 0;
}
