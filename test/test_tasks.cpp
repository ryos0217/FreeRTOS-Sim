#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "stab.h"
}

TEST_GROUP(ClassName)
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


TEST(ClassName, TestName)
{
    setPortMalloc();
    CHECK_EQUAL(pdPASS,xTaskCreate(NULL,"task",10,NULL,0,NULL));
}

int
main (int argc, char *argv[])
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
