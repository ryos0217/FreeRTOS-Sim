######## Build options ########

verbose = 0

######## Build setup ########

# SRCROOT should always be the current directory
SRCROOT         = $(CURDIR)

# .o directory
ODIR            = obj

# Source VPATHS
VPATH           += $(SRCROOT)/Source
VPATH	        += $(SRCROOT)/Source/portable/MemMang
VPATH	        += $(SRCROOT)/Source/portable/GCC/POSIX
VPATH           += $(SRCROOT)/Demo/Common/Full
VPATH           += $(SRCROOT)/Demo/Common/Minimal
VPATH			+= $(SRCROOT)/POSIX/FileIO
VPATH			+= $(SRCROOT)/POSIX/ParTest
VPATH			+= $(SRCROOT)/POSIX

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

# Minimal Demo Objects
C_FILES			+= blocktim.c
C_FILES			+= countsem.c
C_FILES			+= crflash.c
C_FILES			+= crhook.c
C_FILES			+= GenQTest.c
C_FILES			+= QPeek.c
C_FILES			+= recmutex.c

# Full Demo Objects
C_FILES			+= BlockQ.c
C_FILES			+= death.c
C_FILES			+= dynamic.c
C_FILES			+= events.c
C_FILES			+= flop.c
C_FILES			+= integer.c
C_FILES			+= PollQ.c
C_FILES			+= print.c
C_FILES			+= semtest.c

# IO objects
C_FILES			+= fileIO.c
C_FILES			+= ParTest.c

# Main Object
C_FILES			+= main.c

# Include Paths
INCLUDES        += -I$(SRCROOT)/Source/include
INCLUDES        += -I$(SRCROOT)/Source/portable/GCC/POSIX/
INCLUDES        += -I$(SRCROOT)/Demo/Common/include
INCLUDES        += -I$(SRCROOT)/POSIX

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
CWARNS += -Wunused-parameter
CWARNS += -Wunused-value
CWARNS += -Wunused-variable
CWARNS += -Wmissing-prototypes

CFLAGS += -m32
CFLAGS += -DDEBUG=1
CFLAGS += -g -pthread -DUSE_STDIO=1 -D__GCC_POSIX__=1
# Max number of pthreads count. Default value is the dated _POSIX_THREAD_THREADS_MAX which is 64.
CFLAGS += -DMAX_NUMBER_OF_TASKS=300

CFLAGS += $(INCLUDES) $(CWARNS) -O2

######## Makefile targets ########

# Rules
.PHONY : all
all: setup FreeRTOS-Sim

.PHONY : setup
setup:
# Make obj directory
	@mkdir -p $(ODIR)

# Fix to place .o files in ODIR
_OBJS = $(patsubst %,$(ODIR)/%,$(OBJS))

$(ODIR)/%.o: %.c
# If verbose, print gcc execution, else hide
ifeq ($(verbose),1)
	@echo ">> Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<
else
	@echo ">> Compiling $(notdir $<)"
	@$(CC) $(CFLAGS) -c -o $@ $<
endif

FreeRTOS-Sim: $(_OBJS)
	@echo ">> Linking $@..."
ifeq ($(verbose),1)
	$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
else
	@$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
endif

	@echo "-------------------------"
	@echo "BUILD COMPLETE: $@"
	@echo "-------------------------"

.PHONY : clean
clean:
	@-rm -rf $(ODIR) FreeRTOS-Sim
	@echo "--------------"
	@echo "CLEAN COMPLETE"
	@echo "--------------"
