MAKETOP:=..
CPPFLAGS=-I$(MAKETOP)/Project
CPPFLAGS+=-I$(MAKETOP)/Source/
CPPFLAGS+=-I$(MAKETOP)/Source/include/
CPPFLAGS+=-I$(MAKETOP)/Source/portable/GCC/POSIX/
CFLAGS=-g -O0 -Werror -Wall
LDFLAGS=-lpthread

all: test_tasks

list.o: $(MAKETOP)/Source/list.c
	gcc -c -o $@ $(CPPFLAGS) $(CFLAGS) $^

.c.o:
	gcc -c -o $@ $(CPPFLAGS) $(CFLAGS) $^

test_tasks: stub.o list.o tasks_sniffer.o test_tasks.o
	g++ -o $@ $(LDFLAGS) $^ `pkg-config --libs cpputest`

run: test_tasks
	./$< -c -v

clean:
	rm -f test_tasks *~ *.o

.PHONY: clean all run
