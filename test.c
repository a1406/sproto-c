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
#include "common.h"
#include "test.h"

/*
struct nest
{
	char *a;
	bool b;
	uint32_t n_c;
	uint64_t *c;
	uint32_t n_d;
	char **d;
	uint64_t e;
	uint32_t n_f;
	bool *f;
}__attribute__ ((packed));

struct foobar
{
	char *a;
	uint64_t b;
	bool c;
	uint32_t n_d;
	struct nest **d;
	uint32_t n_e;
	char **e;
	uint32_t n_f;
	uint64_t *f;
	uint32_t n_g;
	bool *g;
	uint32_t n_foobar;
	struct foobar **foobar;
}__attribute__ ((packed));

struct t1
{
	char *a;
	uint64_t b;
}__attribute__ ((packed));

struct t2
{
	struct t1 *a;
}__attribute__ ((packed));

struct t3
{
	uint32_t n_b;
	bool *b;
	uint32_t n_a;
	uint64_t *a;
	uint32_t n_c;
	char **c;
}__attribute__ ((packed));
*/
static int encode2(const struct sproto_arg *args)
{
	int ret = 0;
	void **p = (void **)&(args->ud);

	printf("%s tag[%d] type[%d] subtype[%p] value[%p] length[%d] index[%d] mainindex[%d]\n",
		args->tagname, args->tagid, args->type, args->subtype, args->value, args->length,
		args->index, args->mainindex);
	
//	char *p = args->ud;
	switch (args->type)
	{
		case SPROTO_TINTEGER:
		{
			void *data = args->ud;
			if (args->index > 0)
			{
				uint32_t size = *(uint32_t *)data;
				if (size < args->index)
				{
					ret = SPROTO_CB_NIL;
					*p += (sizeof(uint32_t) + sizeof(void *));
					break;
				}
				data += sizeof(uint32_t);
//				data = *(char **)data + sizeof(uint64_t) * (args->index - 1);
				*(uint64_t *)args->value = (*(uint64_t **)data)[args->index - 1];
			}
			else
			{
				*(uint64_t *)args->value = *(uint64_t *)data;
				*p += sizeof(uint64_t);
				printf("encode int value %lu\n", *(uint64_t *)args->value);
			}
			ret = 8;
			break;
		}
		case SPROTO_TBOOLEAN:
		{
			void *data = args->ud;
			if (args->index > 0)
			{
				uint32_t size = *(uint32_t *)data;
				if (size < args->index)
				{
					ret = SPROTO_CB_NIL;
					*p += (sizeof(uint32_t) + sizeof(void *));
					break;
				}
				data += sizeof(uint32_t);
				*(bool *)args->value = (*(bool **)data)[args->index - 1];				
			}
			else
			{
				*(int *)args->value = *(bool *)args->ud;
				*p += sizeof(bool);			
				ret = 8;
			}
			break;
		}
		case SPROTO_TSTRING:
		{
			void *data = args->ud;
			if (args->index > 0)
			{
				uint32_t size = *(uint32_t *)data;
				if (size < args->index)
				{
					ret = SPROTO_CB_NIL;
					*p += (sizeof(uint32_t) + sizeof(void *));
					break;
				}
				data += sizeof(uint32_t);
				char *str = (*(char ***)data)[args->index - 1];
				ret = strlen(str) + 1;
				memcpy(args->value, str, ret);
			}
			else
			{
				ret = strlen(*(char **)(args->ud)) + 1;
				memcpy(args->value, *(char **)args->ud, ret);

				printf("encode string value %s\n", *(char **)(args->ud));
				*p += sizeof(void *);
			}
			break;
		}
		case SPROTO_TSTRUCT:
		{
			void *data = args->ud;
			if (args->index > 0)
			{
				uint32_t size = *(uint32_t *)data;
				if (size < args->index)
				{
					ret = SPROTO_CB_NIL;
					*p += (sizeof(uint32_t) + sizeof(void *));
					break;
				}
				data += sizeof(uint32_t);
				ret = sproto_encode(args->subtype, args->value, args->length, encode2, (*(void ***)data)[args->index - 1]);
			}
			else
			{
				ret = sproto_encode(args->subtype, args->value, args->length, encode2, *(char **)(args->ud));
			}
			break;
		}
	}
	return (ret);
}

static int decode2(const struct sproto_arg *args)
{
	int ret = 0;
	void **p = (void **)&(args->ud);	
//	struct nest *data = args->ud;
	printf("%s tag[%d] type[%d] subtype[%p] value[%p] length[%d] index[%d] mainindex[%d]\n",
		args->tagname, args->tagid, args->type, args->subtype, args->value, args->length,
		args->index, args->mainindex);

	switch (args->type)
	{
		case SPROTO_TINTEGER:
		{
			printf("%lx\n", *(uint64_t *)(args->value));
//			void *data = args->ud;			
//			*(uint64_t *)data = *(uint64_t *)args->value;
//			*p += sizeof(uint64_t);			
			ret = 8;
			break;
		}
		case SPROTO_TBOOLEAN:
		{
			printf("%lx\n", *(uint64_t *)(args->value));
//			void *data = args->ud;			
//			*(bool *)data = *(bool *)args->value;
//			*p += sizeof(bool);			
			ret = 8;
			break;
		}
		case SPROTO_TSTRING:
		{
			printf("%s\n", (char *)(args->value));
//			void *data = args->ud;
//			*(char **)data = strdup((const char *)(args->value));
//			*p += sizeof(void *);			
			break;
		}
		case SPROTO_TSTRUCT:
		{
//			void *data = args->ud;
//			*(void **)data = malloc(10240);
//			ret = sproto_decode(args->subtype, (args->value), args->length, decode, *(void **)data);
			ret = sproto_decode(args->subtype, (args->value), args->length, decode2, NULL);						
			break;
		}
	}
	
	return (0);
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
//	sproto_dump(sp);

	struct foobar foobar;

	struct sproto_type *type1 = sproto_type(sp, "aaa");
	struct sproto_type *type2 = sproto_type(sp, "foobar");
	struct sproto_type *type3 = sproto_type(sp, "foobar.nest");
	struct sproto_type *type4 = sproto_type(sp, "t2");
	struct sproto_type *type5 = sproto_type(sp, "t1");		
	printf("%p %p %p %p %p\n", type1, type2, type3, type4, type5);

	int n1 = sproto_prototag(sp, "aaa");
	int n2 = sproto_prototag(sp, "foobar");
	int n3 = sproto_prototag(sp, "foobar.nest");
	printf("%d %d %d\n", n1, n2, n3);

	struct foobar_nest nest_1;
	nest_1.a = strdup("tangpeilei");
	nest_1.b = true;
	nest_1.n_c = 10;
	nest_1.c = (uint64_t *)malloc(sizeof(uint64_t) * 10);
	int i;
	for (i = 0; i < 10; ++i)
		nest_1.c[i] = i * 0x10;

	nest_1.n_d = 5;
	nest_1.d = (char **)malloc(sizeof(char *) * 5);
	nest_1.d[0] = strdup("aa");
	nest_1.d[1] = strdup("bb");
	nest_1.d[2] = strdup("ccc");
	nest_1.d[3] = strdup("dddddd");
	nest_1.d[4] = strdup("e");
	
	nest_1.e = 0x123ab678;

	nest_1.n_f = 3;
	nest_1.f = (bool *)malloc(sizeof(bool) * 3);
	nest_1.f[0] = true;
	nest_1.f[1] = false;
	nest_1.f[2] = true;	
/*
	int len = sproto_encode(type3, buf, sizeof(buf), encode2, &nest_1);
	printf("encode len = %d\n", len);

	int len2 = sproto_decode(type3, buf, len, decode, NULL);
	printf("decode len = %d\n", len2);

	int len3 = sproto_pack(buf, len, buf2, sizeof(buf2));
	int len4 = sproto_unpack(buf2, len3, buf3, sizeof(buf3));
	printf("len3[%d] len4[%d]\n", len3, len4);

	int len5 = sproto_decode(type3, buf3, len4, decode, NULL);
	printf("len5 = %d\n", len5);

	printf("=========================\n");
	printf("=========================\n");
	struct t2 t2_1;
	t2_1.a = (struct t1 *)malloc(sizeof(struct t1));
	t2_1.a->a = strdup("waimaokeji");
	t2_1.a->b = 0x102030405;

	int len6 = sproto_encode(type4, buf, sizeof(buf), encode2, &t2_1);
	printf("encode len = %d\n", len6);

	struct t2 t2_2;
	
	int len7 = sproto_decode(type4, buf, len6, decode, &t2_2);
	printf("decode len = %d\n", len7);

	void *ttt;
	sproto_decode_c(type4, buf, len6, &ttt);

	fd = open("1.log", O_WRONLY | O_TRUNC | O_CREAT, 00777);
	write(fd, buf, len6);
	close(fd);
*/
	struct sproto_type *type_t4 = sproto_type(sp, "t4");
	struct t4 t4_1;
	t4_1.n_a = 2;
	t4_1.a = malloc(sizeof(void *) * 2);
	t4_1.a[0] = malloc(sizeof(struct t1));
	t4_1.a[0]->a = "123";
	t4_1.a[0]->b = 10;
	t4_1.a[1] = malloc(sizeof(struct t1));
	t4_1.a[1]->a = "456";
	t4_1.a[1]->b = 20;
	
	int len_t4 = sproto_encode(type_t4, buf, sizeof(buf), encode2, &t4_1);
	printf("encode ret %d\n", len_t4);
	sproto_decode(type_t4, buf, sizeof(buf), decode2, NULL);
	struct t4 *ttt4;
	struct pool pool;

	pool_init(&pool);
	int ret = sproto_decode_c(type_t4, buf, len_t4, (void **)&ttt4, &pool);
	printf("decode_c type4 ret %d\n", ret);
	pool_release(&pool);
	
	struct sproto_type *type_t3 = sproto_type(sp, "t3");
	struct t3 t3_1;
	t3_1.a = malloc(sizeof(uint64_t) * 10);
	t3_1.n_a = 10;
	for (i = 0; i < 10; ++i)
		t3_1.a[i] = (i + 1) * 0x10;
	t3_1.b = malloc(sizeof(bool) * 5);
	t3_1.n_b = 5;
	t3_1.b[0] = false;
	t3_1.b[1] = true;
	t3_1.b[2] = false;
	t3_1.b[3] = true;
	t3_1.b[4] = false;

	t3_1.c = malloc(sizeof(char *) * 4);
	t3_1.n_c = 4;
	t3_1.c[0] = "11111";
	t3_1.c[1] = "22222222";
	t3_1.c[2] = "333";
	t3_1.c[3] = "444444";

	t3_1.n_d = 3;
	t3_1.d = malloc(sizeof(void *) * 3);
	t3_1.d[0] = malloc(sizeof(struct t1));
	t3_1.d[1] = malloc(sizeof(struct t1));
	t3_1.d[2] = malloc(sizeof(struct t1));

	t3_1.d[0]->a = "d0.a";
	t3_1.d[1]->a = "d1.a";
	t3_1.d[2]->a = "d1.a";
	t3_1.d[0]->b = 10;
	t3_1.d[1]->b = 20;
	t3_1.d[2]->b = 30;
	
	int len_t3 = sproto_encode(type_t3, buf, sizeof(buf), encode2, &t3_1);
	struct t3 *ttt;
	pool_init(&pool);
	ret = sproto_decode_c(type_t3, buf, len_t3, (void **)&ttt, &pool);
	printf("decode_c typet3 ret %d\n", ret);	
	pool_release(&pool);	
    return 0;
}
