#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char **getline();
char *getcwd(char *buf, size_t size);

int main() {
  int i;
  char **input;

  while (1) {
    printf("\nMy Shell>> ");
    input = getline();

    /* handling all cases of different commands */
    for (int i = 0; input[i] != NULL; ++i) {

      /* exit the shell */
      if ( strcmp(input[0],"exit") == 0 ) {
	exit(-1);
      }

      /* print current directory */
      if (strcmp(input[0],"cwd") == 0 ) {
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	    printf("%s\n",cwd);
	} /* else {
	    perror("getcwd() Error");
	} */
      }

      /* changing current directory */
      if ( strcmp(input[0],"cd") == 0 ) {
	chdir(input[1]);
      }



      printf("Item %i of input: %s\n", i, input[i]); // The input list that must be parsed. 
    }
  }
  
  return 0;
}
