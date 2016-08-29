all: proto test

CFLAGS = -g -O0 -I/usr/include/lua5.2
LDFLAGS = -lprotobuf-c  -llua5.2

test: test.c ../sproto.c ../sprotoc_encode.c
	gcc -o $@ $(CFLAGS) $^ $(LDFLAGS) 

proto:
	rm -f 1.spb
	cd .. && ./spc.sh readlua/proto.sproto readlua/1.spb
	../generate_head 1.spb  > test.h

clean:
	rm -f test
