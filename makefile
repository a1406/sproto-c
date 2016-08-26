all: test proto

CFLAGS = -g -O0
LDFLAGS = 

test: test.c sproto.c
	gcc  -o $@ $(CFLAGS) $(LDFLAGS) $^

proto: testall.sproto
	rm 1.spb
	./spc.sh testall.sproto 1.spb

generate_head: generate_head.c
	gcc  -o $@ $(CFLAGS) $(LDFLAGS) $^
