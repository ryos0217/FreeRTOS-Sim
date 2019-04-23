MAKETOP:=..
CPPFLAGS=-I$(MAKETOP)/Project -I$(MAKETOP)/Source/include/ -I$(MAKETOP)/Source/portable/GCC/POSIX/
CFLAGS=-g -O0 -Werror -Wall
LDFLAGS=-lpthread

all: test_tasks

tasks.o: $(MAKETOP)/Source/tasks.c
	gcc -c -o $@ $(CPPFLAGS) $(CFLAGS) $^

list.o: $(MAKETOP)/Source/list.c
	gcc -c -o $@ $(CPPFLAGS) $(CFLAGS) $^

.c.o:
	gcc -c -o $@ $(CPPFLAGS) $(CFLAGS) $^

test_tasks: stub.o list.o tasks.o test_tasks.o
	g++ -o $@ $(LDFLAGS) $^ `pkg-config --libs cpputest`

run: test_tasks
	./$< -c -v

clean:
	rm -f test_tasks *~ *.o

.PHONY: clean all run
