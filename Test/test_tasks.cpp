#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "stub.h"

static void prvCheckTask( void *pvParameters )
{
  int *ptr = (int*)pvParameters;
  *ptr = 0;
}
}

#define DEFAULT_STACK_SIZE 8192

TEST_GROUP(xTaskCreate)
{
  TaskHandle_t task;

  void setup()
  {
  }
  void teardown()
  {
    vTaskDelete (task);
    stub_reset ();
  }
};

TEST(xTaskCreate, FailsWhenAllocFail)
{
  stub_set_malloc_fail ();
  CHECK_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY,
              xTaskCreate (prvCheckTask, "TaskName", DEFAULT_STACK_SIZE, NULL, 0, &task));
}

TEST(xTaskCreate, FailsWhenInvalidTaskDepth)
{
  CHECK_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY,
              xTaskCreate (prvCheckTask, "TaskName", 0, NULL, 0, &task));
}

TEST(xTaskCreate, SuccessEvenWithoutHandler)
{
  CHECK_EQUAL(pdPASS,
              xTaskCreate (NULL, "TaskName", DEFAULT_STACK_SIZE, NULL, 0, &task));
}

TEST(xTaskCreate, HandlerIsCalledWithParam)
{
  int param = -1;
  void *pvParameters = &param;

  CHECK_EQUAL(pdPASS,
              xTaskCreate (prvCheckTask, "TaskName", DEFAULT_STACK_SIZE, pvParameters, 0, &task));
  CHECK_EQUAL(0, param);
}

int
main (int argc, char *argv[])
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
