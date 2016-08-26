#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include "sproto.h"

int main(int argc, char *argv[])
{
	static char buf[4096000];	
	int fd = open(argv[0], O_RDONLY);
	struct sproto *sp = sproto_create(&buf[0], size);
	close(fd);
	
    return 0;
}
