CXXFLAGS=-g -I../Source/include -I../Project -I../Source/portable/GCC/POSIX
CC:=clang
CXX:=clang++


all:	test_tasks

test_tasks.o:	test_tasks.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $^

test_tasks:	test_tasks.o
	$(CXX) -o $@ $(CXXFLAGS) $^ `pkg-config --libs cpputest`

run:	test_tasks
	./$< -c -v

clean:
	rm -f test_tasks *~ *.o

.PHONY:all clean run