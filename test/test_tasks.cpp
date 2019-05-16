#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>


extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

TEST_GROUP(ClassName)
{
  void setup()
  {
  }
  void teardown()
  {
    ;
  }
};


TEST(ClassName, TestName)
{
    xTaskCreate(NULL,NULL,0,NULL,0,NULL);
    FAIL("first test");
}

int
main (int argc, char *argv[])
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
