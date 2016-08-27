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

int test_protobuf()
{
	static char buf[4096];
	Proto proto;
	proto__init(&proto);
	proto.int1 = 10000;
	proto.str1 = "tangpeilei";
	proto.bool1 = true;
	proto.int2 = 0x1234567890abcdef;
	SubProto *psub[3];
	proto.n_sub = 3;
	proto.sub = psub;
	
	SubProto sub[3];
	int int1[3] = {1,2,3};
	char *str1[3];
	str1[0] = "aaa";
	str1[1] = "bbb";
	str1[2] = "ccc";	
	int bool1[3] = {1, 0, 1};
	uint64_t int2[3] = {1111111, 22222222, 333333333};
	int i;
	for (i = 0; i < 3; ++i)
	{
		psub[i] = &sub[i];
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
	int len = proto__pack(&proto, buf);
	Proto *unpack = proto__unpack(NULL, len, buf);
	proto__free_unpacked(unpack, NULL);
	return len;
}

int test_sproto(struct sproto_type *type)
{
	static char buf[4096];
	struct proto proto;
	proto.int1 = 10000;
	proto.str1 = "tangpeilei";
	proto.bool1 = true;
	proto.int2 = 0x1234567890abcdef;
	struct subproto *psub[3];
	proto.n_sub = 3;
	proto.sub = psub;
	
	struct subproto sub[3];
	uint64_t int1[3] = {1,2,3};
	char *str1[3];
	str1[0] = "aaa";
	str1[1] = "bbb";
	str1[2] = "ccc";	
	bool bool1[3] = {true, false, true};
	uint64_t int2[3] = {1111111, 22222222, 333333333};
	int i;
	for (i = 0; i < 3; ++i)
	{
		psub[i] = &sub[i];
		sub[i].n_int1 = 3;
		sub[i].int1 = int1;
		sub[i].n_str1 = 3;
		sub[i].str1 = str1;
		sub[i].n_bool1 = 3;
		sub[i].bool1 = bool1;
		sub[i].n_int2 = 3;
		sub[i].int2 = int2;
	}
	struct pool pool;	
	pool_init(&pool);
	int len = sproto_encode(type, buf, sizeof(buf), sprotoc_encode, &proto);
	struct proto *unpack;
	int ret = sproto_decode_c(type, buf, len, (void **)&unpack, &pool);
	pool_release(&pool);		
	return (len);
}

int main(int argc, char *argv[])
{
	static char buf[4096];
	static char buf2[4096];
	static char buf3[4096];		
	int fd = open("1.spb", O_RDONLY);
	size_t size =  read(fd, buf, sizeof(buf));
	struct sproto *sp = sproto_create(&buf[0], size);
	close(fd);

	struct sproto_type *type = sproto_type(sp, "proto");		

	int len_protobuf, len_sproto;
	
	int i;
	for (i = 0; i < 1000000; ++i)
		len_protobuf = test_protobuf();

	for (i = 0; i < 1000000; ++i)
		len_protobuf = test_sproto(type);	
	
    return 0;
}
