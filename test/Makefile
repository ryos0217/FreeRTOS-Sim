CXXFLAGS=-g -I../Source/include -I../Project -I../Source/portable/GCC/POSIX
CC:=clang
CXX:=clang++


all:	test_tasks

test_tasks.o:	test_tasks.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $^

test_tasks:	test_tasks.o
	$(CXX) -o $@ $(CPPFLAGS) $^ `pkg-config --libs cpputest`

run:	test_tasks
	./$< -c -v

clean:
	rm -f test_tasks *~ *.o

.PHONY:all clean run