Cmake_minimum_required(VERSION 3.10)

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

add_executable(test_tasks
  test_tasks.cpp
  stub.c
  ../Source/list.c
  tasks_sniffer.c)

target_link_libraries(
  test_tasks
  CppUTest
  CppUTestExt)
