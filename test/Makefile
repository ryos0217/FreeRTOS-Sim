CPPFLAGS=-I../Source/include

all:	test_tasks

test_tasks.o:	test_tasks.cpp
	g++ -c -o $@ $(CPPFLAGS) $^

test_tasks:	test_tasks.o
	g++ -o $@ $(CPPFLAGS) $^ `pkg-config --libs cpputest`

run:	test_tasks
	./$<

clean:
	rm -f test_tasks *~ *.o

.PHONY:all clean run