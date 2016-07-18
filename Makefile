TARGETS=bin/cc-01 bin/cc-02 bin/cc-03

all: $(TARGETS) test

bin/cc-01: cc-01.o
bin/cc-02: cc-02.o
bin/cc-03: cc-03.o

bin/%:
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(TARGETS) *.o

test:
	./tests-run-01.sh
