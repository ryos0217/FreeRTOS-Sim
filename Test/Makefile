######## Build options ########

verbose = 0
ifeq ($(verbose),1)
S=
else
S=@
endif

######## Build setup ########

# SRCROOT should always be the current directory
SRCROOT         = $(CURDIR)/../

# .o directory
ODIR            = out

# Source VPATHS
VPATH           += $(SRCROOT)/Test
VPATH           += $(SRCROOT)/Source
VPATH	        += $(SRCROOT)/Source/portable/MemMang
VPATH	        += $(SRCROOT)/Source/portable/GCC/POSIX
VPATH           += $(SRCROOT)/Demo/Common
VPATH			+= $(SRCROOT)/Project/FileIO
VPATH			+= $(SRCROOT)/Project

# Test Objects
C_FILES			+= test_apis.cpp
C_FILES			+= test_tasks.cpp


# FreeRTOS Objects
C_FILES			+= croutine.c
C_FILES			+= event_groups.c
C_FILES			+= list.c
C_FILES			+= queue.c
C_FILES			+= tasks.c
C_FILES			+= timers.c

# portable Objects
C_FILES			+= heap_3.c
C_FILES			+= port.c

# Demo Objects
C_FILES			+= Minimal/blocktim.c
C_FILES			+= Minimal/countsem.c
C_FILES			+= Minimal/GenQTest.c
C_FILES			+= Minimal/QPeek.c
C_FILES			+= Minimal/recmutex.c
C_FILES			+= Full/BlockQ.c
C_FILES			+= Full/death.c
C_FILES			+= Full/dynamic.c
C_FILES			+= Full/flop.c
C_FILES			+= Full/integer.c
C_FILES			+= Full/PollQ.c
C_FILES			+= Full/semtest.c
C_FILES			+= Full/print.c

C_FILES			+= Minimal/AbortDelay.c
C_FILES			+= Minimal/EventGroupsDemo.c
C_FILES			+= Minimal/IntSemTest.c
C_FILES			+= Minimal/QueueSet.c
C_FILES			+= Minimal/QueueSetPolling.c
C_FILES			+= Minimal/QueueOverwrite.c
C_FILES			+= Minimal/TaskNotify.c
C_FILES			+= Minimal/TimerDemo.c

# Main Object
C_FILES			+= main.c

# Include Paths
INCLUDES        += -I$(SRCROOT)/Source/include
INCLUDES        += -I$(SRCROOT)/Source/portable/GCC/POSIX/
INCLUDES        += -I$(SRCROOT)/Demo/Common/include
INCLUDES        += -I$(SRCROOT)/Project
INCLUDES        += -I$(SRCROOT)/Source/

# Generate OBJS names
OBJS = $(patsubst %.c,%.o,$(C_FILES))

######## C Flags ########

# Warnings
CWARNS += -W
CWARNS += -Wall
CWARNS += -Werror
CWARNS += -Wextra
CWARNS += -Wformat
CWARNS += -Wmissing-braces
CWARNS += -Wno-cast-align
CWARNS += -Wparentheses
CWARNS += -Wshadow
CWARNS += -Wno-sign-compare
CWARNS += -Wswitch
CWARNS += -Wuninitialized
CWARNS += -Wunknown-pragmas
CWARNS += -Wunused-function
CWARNS += -Wunused-label
CWARNS += -Wno-unused-parameter
CWARNS += -Wunused-value
CWARNS += -Wunused-variable
CWARNS += -Wno-missing-prototypes

#CWARNS += -Wno-unused-function

# TODO: To be built with 32bit,
#       but cpputest apt package contains only 64bit binaries.
# CFLAGS += -m32

CFLAGS += -DDEBUG=1
#CFLAGS += -g -DUSE_STDIO=1 -D__GCC_POSIX__=1
CFLAGS += -g -UUSE_STDIO -D__GCC_POSIX__=1
ifneq ($(shell uname), Darwin)
CFLAGS += -pthread
endif

# MAX_NUMBER_OF_TASKS = max pthreads used in the POSIX port.
# Default value is 64 (_POSIX_THREAD_THREADS_MAX), the minimum number required by POSIX.
CFLAGS += -DMAX_NUMBER_OF_TASKS=300

CFLAGS += $(INCLUDES) $(CWARNS) -O2

# TODO: Dirty hack to re-use CFLAGS as much as it was.
CXXFLAGS = $(filter-out -W%missing-prototypes,$(CFLAGS))

LINKFLAGS=$(shell pkg-config --libs cpputest)


######## Makefile targets ########

# Rules
.PHONY : all
all: $(ODIR)/test_apis $(ODIR)/test_tasks

# Fix to place .o files in ODIR
_OBJS = $(patsubst %,$(ODIR)/%,$(OBJS))


$(ODIR)/%.o: %.c
	@echo ">> Compiling $(notdir $<)"
	@mkdir -p $(dir $@)
	$(S)$(CC) $(CFLAGS) -c -o $@ $<

$(ODIR)/%.o: %.cpp
	@echo ">> Compiling $(notdir $<)"
	@mkdir -p $(dir $@)
	$(S)$(CXX) $(CXXFLAGS) -c -o $@ $<

define link
	@echo ">> Linking $@..."
	$(S)$(CXX) $(CXXFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
	@echo "BUILD COMPLETE: $@"
endef

test_apis_OBJS = \
	$(ODIR)/heap_3.o \
	$(ODIR)/port.o \
	$(ODIR)/list.o \
	$(ODIR)/queue.o \
	$(ODIR)/timers.o \
	$(ODIR)/tasks.o \
	$(ODIR)/test_apis.o
$(ODIR)/test_apis: $(test_apis_OBJS)
	$(link)

test_tasks_OBJS = \
	$(ODIR)/stub.o \
	$(ODIR)/list.o \
	$(ODIR)/tasks_sniffer.o \
	$(ODIR)/test_tasks.o
$(ODIR)/test_tasks: $(test_tasks_OBJS)
	$(link)

.PHONY : clean
clean:
	$(S)-rm -rf $(ODIR) *~
	@echo "CLEAN COMPLETE"

.PHONY: run
run: all
	$(ODIR)/test_apis -c -v
	$(ODIR)/test_tasks -c -v
	@echo "TEST COMPLETE"

.PHONY: style
style_FILES = $(wildcard *.c *.h *.cpp)
style:
	$(S)clang-format -i $(style_FILES)
	@echo "STYLE COMPLETE"
