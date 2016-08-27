all: proto test

CFLAGS = -g -O0
LDFLAGS = -lprotobuf-c

proto:
	protoc-c --c_out=. proto.proto  
	rm -f 1.spb
	cd .. && ./spc.sh test_protobuf/proto.sproto test_protobuf/1.spb
	../generate_head 1.spb  > test.h

test: test.c proto.pb-c.c ../sproto.c ../sprotoc_encode.c
	gcc  -o $@ $(CFLAGS) $^ $(LDFLAGS) 

clean:
	rm -f test
