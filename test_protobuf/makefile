all: proto test

CFLAGS = -g -O0
LDFLAGS = 

proto:
	protoc-c --c_out=. proto.proto  
	rm -f 1.spb
	cd .. && ./spc.sh test_protobuf/proto.sproto test_protobuf/1.spb
	../generate_head 1.spb  > test.h

test: test.c
	gcc  -o $@ $(CFLAGS) $(LDFLAGS) $^
