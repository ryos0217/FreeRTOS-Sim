CPPFLAGS=-g -I../Source/include -I../Project -I../Source/portable/GCC/POSIX

all:	test_tasks

tasks.o:	../Source/tasks.c
	gcc -c -o $@ $(CPPFLAGS) $^

stab.o:	stab.c
	gcc -c -o $@ $(CPPFLAGS) $^

test_tasks.o:	test_tasks.cpp 
	g++ -c -o $@ $(CPPFLAGS) $^

test_tasks:	test_tasks.o	tasks.o	stab.o
	g++ -o $@ $(CPPFLAGS) $^ `pkg-config --libs cpputest`

run:	test_tasks
	./$<

clean:
	rm -f test_tasks *~ *.o

.PHONY:all clean run