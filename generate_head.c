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
#include "sprotoc_common.h"

static int escape_type_name(char *name)
{
	while(*name)
	{
		if (*name == '.')
			*name = '_';
		++name;
	}
	return (0);
}

static int	print_field(struct field *f)
{
	char c = ' ';
	if (f->type & SPROTO_TARRAY)
	{
		c = '*';
		printf("	uint32_t n_%s; //%d\n", f->name, f->tag);
	}
	f->type &= ~SPROTO_TARRAY;
	switch (f->type)
	{
		case SPROTO_TINTEGER:
			printf("	uint64_t %c%s; //%d\n", c, f->name, f->tag);
			break;
		case SPROTO_TBOOLEAN:
			printf("	bool %c%s; //%d\n", c, f->name, f->tag);			
			break;
		case SPROTO_TSTRING:
			printf("	char %c*%s; //%d\n", c, f->name, f->tag);						
			break;
		case SPROTO_TSTRUCT:
			printf("	struct %s %c*%s; //%d\n", f->st->name, c, f->name, f->tag);						
			break;			
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

	char *name = strdup(argv[1]);
	escape_type_name(name);

	printf("#ifndef _%s_H__\n", name);
	printf("#define _%s_H__\n\n", name);

	int i;
	for (i=0;i<sp->type_n;i++) {
		escape_type_name((char *)sp->type[i].name);
		printf("struct %s;\n", sp->type[i].name);
	}
	
	for (i=0;i<sp->type_n;i++) {
		printf("struct %s\n", sp->type[i].name);
		printf("{\n");
//		if (strcmp(type_name, sp->type[i].name) == 0) {
//			return &sp->type[i];
//		}
		int j;
		for (j = 0; j < sp->type[i].n; ++j) {
			print_field(&sp->type[i].f[j]);
		}

		printf("}__attribute__ ((packed));\n\n");
	}

	printf("#endif\n");
	
    return 0;
}
