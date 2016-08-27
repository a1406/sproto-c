#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include "../sproto.h"
#include "../sprotoc_common.h"
#include "test.h"
#include "proto.pb-c.h"

int main(int argc, char *argv[])
{
	static char buf[4096];
	static char buf2[4096];
	static char buf3[4096];		
	int fd = open("1.spb", O_RDONLY);
	size_t size =  read(fd, buf, sizeof(buf));
	struct sproto *sp = sproto_create(&buf[0], size);
	close(fd);

	Proto proto;
	proto__init(&proto);
	proto.int1 = 10000;
	proto.str1 = "tangpeilei";
	proto.bool1 = true;
	proto.int2 = 0x1234567890abcdef;
	proto.n_sub = 3;
	SubProto sub[3];
	int int1[3];
	char **str1[3];
	int bool1[3];
	uint64_t int2[3];
	int i;
	for (i = 0; i < 3; ++i)
	{
		sub_proto__init(&sub[i]);
		sub[i].n_int1 = 3;
		sub[i].int1 = int1;
		sub[i].n_str1 = 3;
		sub[i].str1 = str1;
		sub[i].n_bool1 = 3;
		sub[i].bool1 = bool1;
		sub[i].n_int2 = 3;
		sub[i].int2 = int2;
	}
	
    return 0;
}
