#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>

extern char **getline();
char *getcwd(char *buf, size_t size);
char cwd[1024];


int main() {
  int i;
  char **input;

  while (1) {

    if ( getcwd(cwd, sizeof(cwd) ) != NULL) {
        printf("\nYou@My_Shell>> %s: ", cwd);
    } else {
	printf("\nMy Shell>> ");
    }
    input = getline();

    /* handling all cases of different commands */
    for (int i = 0; input[i] != NULL; ++i) {

      /* exit the shell */
      if ( strcmp(input[0],"exit") == 0 ) {
	exit(-1);
      }

      /* print current directory */
      if (strcmp(input[0],"cwd") == 0 ) {
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	    printf("%s\n",cwd);
	} else {
	    perror("getcwd() Error");
	} 
      }

      /* changing current directory */
      if ( strcmp(input[0],"cd") == 0 ) {
	chdir(input[1]);
      }

      /* list files in a directory */
      if ( strcmp(input[0],"ls") == 0 ) {
	char *pointer = NULL;
	DIR *dp = NULL;
	struct dirent *sd = NULL;
	pointer = getenv("PWD");
	dp = opendir((const char*)pointer);
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
