#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "stab.h"
}

TEST_GROUP(xTasksCreateTest)
{
  void setup()
  {
      resetPortMalloc();
  }
  void teardown()
  {
    ;
  }
};


TEST(xTasksCreateTest, setPortMallocFaild)
{
    resetPortMalloc();
    CHECK_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY,xTaskCreate(NULL,"task",10,NULL,0,NULL));
}

TEST(xTasksCreateTest, setPortMallocSuccess)
{
    setPortMalloc();
    CHECK_EQUAL(pdPASS,xTaskCreate(NULL,"task",10,NULL,0,NULL));
}

int
main (int argc, char *argv[])
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
