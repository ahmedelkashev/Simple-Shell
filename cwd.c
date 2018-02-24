/* print current working directory */
#include <stdio.h>

char *getcwd(char *buf, size_t size);
char cwd[1024];

int main() {
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("%s\n",cwd);
	} else {
		perror("getcwd() Error");
	}
}
