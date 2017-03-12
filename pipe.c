#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

/* WARNING. No error handling */
char *read_entire_file(char *name)
	{
	FILE *fh = fopen(name, "rb");
	fseek(fh, 0, SEEK_END);
	long fsize = ftell(fh);
	fseek(fh, 0, SEEK_SET);

	char *buffer = malloc(fsize + 1);
	fread(buffer, fsize, 1, fh);
	fclose(fh);
	buffer[fsize] = 0;

	return buffer;
	}

int main(void)
	{
    int pfds[2];

	char *license = read_entire_file("LICENSE");
	long fsize = strlen(license); /* Yes. I know */
	char *buffer = malloc(fsize + 1);

    pipe(pfds);

    if (!fork())
		{
        printf(" CHILD: writing to the pipe\n");
        write(pfds[1], license, fsize + 1);
        printf(" CHILD: exiting\n");
        exit(0);
		}
	else
		{
        printf("PARENT: reading from pipe\n");
        read(pfds[0], buffer, fsize + 1);
        printf("PARENT: read \"%s\"\n", buffer);
        wait(NULL);
    	}

    return 0;
	}
