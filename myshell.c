#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char **getline();

int main() {
  int i;
  char **input;

  while (1) {
    input = getline();

    /* handling all cases of different commands */
    for (int i = 0; input[i] != NULL; ++i) {

      /* exit the shell */
      if ( strcmp(input[0],"exit") == 0 ) {
	exit(-1);
      }


      printf("Item %i of input: %s\n", i, input[i]); // The input list that must be parsed. 
    }
  }
  
  return 0;
}
