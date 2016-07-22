TARGETS=bin/cc-01 bin/cc-02 bin/cc-03

all: $(TARGETS) test

bin/cc-01: cc-01.o hex.o b64.o util.o
bin/cc-02: cc-02.o xor.o hex.o util.o
bin/cc-03: cc-03.o xor.o hex.o util.o

bin/%:
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(TARGETS) *.o
test:
	./tests-run-01.sh
