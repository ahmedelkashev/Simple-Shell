#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
extern char **getline();
int main() {
 int i;
 char **input;
 while (1) {
 input = getline();
 for (int i = 0; input[i] != NULL; ++i) {
// The input list that must be parsed.
 printf("Item %i of input: %s\n", i, input[i]);
 }
 }
 return 0;
}
