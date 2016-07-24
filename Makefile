TARGETS=bin/cc-01 bin/cc-02 bin/cc-03 bin/cc-04 bin/cc-05

all: $(TARGETS) test

bin/cc-01: cc-01.o hex.o b64.o util.o
bin/cc-02: cc-02.o xor.o hex.o util.o
bin/cc-03: cc-03.o xor.o hex.o util.o
bin/cc-04: cc-04.o xor.o hex.o util.o
bin/cc-05: cc-05.o xor.o hex.o util.o

bin/%:
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(TARGETS) *.o
test:
	./tests-run-01.sh
