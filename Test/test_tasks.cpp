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

TEST_GROUP(xTaskCreate)
{
  void setup()
  {
  }
  void teardown()
  {
    stub_reset ();
  }
};

TEST(xTaskCreate, allocFail)
{
  stub_set_malloc_fail ();
  CHECK_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY,
              xTaskCreate (NULL, "TaskName", 0, NULL, 0, NULL));
}

TEST(xTaskCreate, SuccessEvenWithoutHandler)
{
  CHECK_EQUAL(pdPASS,
              xTaskCreate (NULL, "TaskName", 0, NULL, 0, NULL));
}

TEST(xTaskCreate, DeleteCreatedTask)
{
  TaskHandle_t task;

  CHECK_EQUAL(pdPASS,
              xTaskCreate (NULL, "TaskName", 0, NULL, 0, &task));

  vTaskDelete (task);
}

TEST(xTaskCreate, HandlerIsCalledWithParam)
{
  int param = -1;
  void *pvParameters = &param;

  CHECK_EQUAL(pdPASS,
              xTaskCreate (prvCheckTask, "TaskName", 0, pvParameters, 0, NULL));
  CHECK_EQUAL(0, param);
}

int
main (int argc, char *argv[])
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
