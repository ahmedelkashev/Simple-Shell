#include <dirent.h>
#include <stdio.h>

int main() {
	DIR *dp = NULL;
	struct dirent *sd = NULL;
	dp = opendir(".");
	while( (sd=readdir(dp)) != NULL ) {
		printf("%4s \t", sd->d_name);
	}
	printf("\n");
}
