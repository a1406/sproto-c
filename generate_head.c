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

#define SPROTO_TARRAY 0x80
#define CHUNK_SIZE 1000
#define SIZEOF_LENGTH 4
#define SIZEOF_HEADER 2
#define SIZEOF_FIELD 2

struct field {
	int tag;
	int type;
	const char * name;
	struct sproto_type * st;
	int key;
};

struct sproto_type {
	const char * name;
	int n;
	int base;
	int maxn;
	struct field *f;
	int c_size;
};

struct protocol {
	const char *name;
	int tag;
	struct sproto_type * p[2];
};

struct chunk {
	struct chunk * next;
};

struct pool {
	struct chunk * header;
	struct chunk * current;
	int current_used;
};

struct sproto {
	struct pool memory;
	int type_n;
	int protocol_n;
	struct sproto_type * type;
	struct protocol * proto;
};

int escape_type_name(char *name)
{
	while(*name)
	{
		if (*name == '.')
			*name = '_';
		++name;
	}
	return (0);
}

int main(int argc, char *argv[])
{
	static char buf[4096000];	
	int fd = open(argv[1], O_RDONLY);
	size_t size =  read(fd, buf, sizeof(buf));
	struct sproto *sp = sproto_create(&buf[0], size);
	close(fd);

	if (!sp)
		return (0);

	printf("#ifndef %s_H__\n", argv[1]);
	printf("#define %s_H__\n\n", argv[1]);	

	int i;
	for (i=0;i<sp->type_n;i++) {
		escape_type_name((char *)sp->type[i].name);
		printf("struct %s\n", sp->type[i].name);
		printf("{\n");
//		if (strcmp(type_name, sp->type[i].name) == 0) {
//			return &sp->type[i];
//		}

		printf("}__attribute__ ((packed))\n\n");
	}

	printf("#endif\n");
	
    return 0;
}
