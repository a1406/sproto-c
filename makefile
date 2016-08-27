all: proto generate_head test 

CFLAGS = -g -O0
LDFLAGS = 

test: test.c sproto.c sprotoc_encode.c
	gcc  -o $@ $(CFLAGS) $(LDFLAGS) $^

proto: testall.sproto
	rm 1.spb
	./spc.sh testall.sproto 1.spb

generate_head: generate_head.c sproto.c
	gcc  -o $@ $(CFLAGS) $(LDFLAGS) $^
