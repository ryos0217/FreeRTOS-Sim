cmake_minimum_required(VERSION 3.5)

add_compile_options(
  -W
  -Wall
  -Werror
  -Wextra
  -Wformat
  -Wmissing-braces
  -Wno-cast-align
  -Wparentheses
  -Wshadow
  -Wno-sign-compare
  -Wswitch
  -Wuninitialized
  -Wunknown-pragmas
  -Wunused-function
  -Wunused-label
  -Wunused-value
  -Wunused-variable
  -Wno-unused-parameter
  -DDEBUG=1
  -UUSE_STDIO
  -D__GCC_POSIX__=1
  -DMAX_NUMBER_OF_TASKS=300)

set(CMAKE_C_FLAGS
  ${CMAKE_C_FLAGS}
  -Wno-missing-prototypes)

include_directories(
  ../Project
  ../Source
  ../Source/include
  ../Source/portable/GCC/POSIX)

add_executable(test_apis
  test_apis.cpp
  ../Source/portable/MemMang/heap_3.c
  ../Source/portable/GCC/POSIX/port.c
  ../Source/list.c
  ../Source/queue.c
  ../Source/timers.c
  ../Source/tasks.c)

target_link_libraries(
  test_apis
  pthread
  CppUTest
  CppUTestExt)

set(test_tasks_SRCS
  test_tasks.cpp
  stub.c
  ../Source/list.c
  tasks_sniffer.c)

set(test_tasks_LIBS
  CppUTest
  CppUTestExt)

 add_executable(test_tasks
  ${test_tasks_SRCS})

target_link_libraries(
  test_tasks
  ${test_tasks_LIBS})

add_executable(test_tasks_SUPPORT_STATIC_ALLOCATION
  ${test_tasks_SRCS})

target_link_libraries(
  test_tasks_SUPPORT_STATIC_ALLOCATION
  ${test_tasks_LIBS})

target_compile_options(
  test_tasks_SUPPORT_STATIC_ALLOCATION
  PRIVATE -DconfigSUPPORT_STATIC_ALLOCATION=1)
