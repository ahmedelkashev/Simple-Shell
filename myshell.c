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
char program_path[1024];

int main(int argc, char *argv[]) {
  int i;
  char **input;

  while (1) {
    
    /* get current working directory and prompt */
    if ( getcwd(cwd, sizeof(cwd) ) != NULL) {
        printf("You@My_Shell:%s # ", cwd);
    } else {
	printf("\nMy Shell>> ");
    }

    input = getline();

    int writeFile;

    /* make sure that there are arguments */
    if (argc > 0) {
	
	int pid = fork();

	/* parent process-shell */
	if (pid > 0) { 
	    wait(NULL);
	}
	/* child process */
	else {

	    /* if < is in command, redirect output to file */
   	    for (int i = 0; input[i] != NULL; ++i) {
		if( strcmp(input[i],">") == 0 ) {
			writeFile = open(input[i+1], O_RDWR | O_CREAT, 
S_IRUSR | S_IWUSR);
			input[i] = NULL;
			input[i+1] = NULL;
			dup2(writeFile, 1);
			dup2(writeFile, 2);
			close(writeFile);
		}
	    }

	    /* add path to the program and execute */
	    sprintf(program_path,"/bin/%s",input[0]);
	    execvp(program_path,&input[0]);
	    exit(1);
	}
    }

    /* my implementation of cd and exit commands */
    for (int i = 0; input[i] != NULL; ++i) {


      /* exit the shell */
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


      printf("Item %i of input: %s\n", i, input[i]); // The input list that must be parsed. 
    }
  }
  
  return 0;
}
