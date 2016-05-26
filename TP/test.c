#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <pwd.h>
#include <string.h>
#include <utime.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>

int main(){


	int test;
	char* lol=malloc(sizeof(char)*50);
	test=open("blbl.txt", O_WRONLY);
	dup2(test,1);
	printf("loloooooollo");
}
