#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>

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
        printf("\nYou@My_Shell>>%s: ", cwd);
    } else {
	printf("\nMy Shell>> ");
    }

    input = getline();


    /* handling all cases of different commands */
    for (int i = 0; input[i] != NULL; ++i) {

      /* commands with no arguments */
      if (argc == 1) {
	int pid = fork();

	/* parent process-shell */
	if (pid > 0) { 
	    wait(NULL);
	}
	/* child process */
	else {
	    sprintf(program_path,"./%s",input[0]);
	    puts(program_path);
	    execvp(program_path,argv);
	    exit(1);
	}
      }


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

      /* list files in a directory */
      if ( strcmp(input[0],"ls") == 0 ) {
	DIR *dp = NULL;
	struct dirent *sd = NULL;
	dp = opendir(".");
	while( (sd=readdir(dp)) != NULL ) {
	    printf("%s \t", sd->d_name);
	}
	printf("\n");
      }

      printf("Item %i of input: %s\n", i, input[i]); // The input list that must be parsed. 
    }
  }
  
  return 0;
}
