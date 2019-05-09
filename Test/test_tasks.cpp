#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

extern "C" {
#include "FreeRTOS.h"
#include "stub.h"
#include "task.h"
#include "tasks_sniffer.h"

static void prvCheckTask(void* pvParameters)
{
  int* ptr = (int*)pvParameters;
  *ptr = 0;

  while (1) {
    /* Task shall not end */
  }
}

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize)
{
}
}

#define DEFAULT_STACK_DEPTH 2048

TEST_GROUP(xTaskCreate)
{
  TaskHandle_t task;

  void setup()
  {
  }
  void teardown()
  {
    vTaskDelete(task);
    stub_reset();
  }
};

TEST(xTaskCreate, FailsWhenAllocFail)
{
  stub_set_malloc_fail();
  CHECK_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY,
      xTaskCreate(prvCheckTask, "TaskName", DEFAULT_STACK_DEPTH, NULL, 1, &task));
}

TEST(xTaskCreate, FailsWhenInvalidTaskDepth)
{
  CHECK_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY,
      xTaskCreate(prvCheckTask, "TaskName", 0, NULL, 1, &task));
}

TEST(xTaskCreate, SuccessEvenWithoutHandler)
{
  CHECK_EQUAL(pdPASS,
      xTaskCreate(NULL, "TaskName", DEFAULT_STACK_DEPTH, NULL, 1, &task));
}

TEST(xTaskCreate, CreatesTaskWithRunningState)
{
  int ret = xTaskCreate(prvCheckTask, "TaskName", DEFAULT_STACK_DEPTH, NULL, 1, &task);

  CHECK_EQUAL(pdPASS, ret);
  CHECK_EQUAL(eRunning, eTaskGetState(task));
}

TEST(xTaskCreate, CreatesTaskWithSpecifiedName)
{
  int ret = xTaskCreate(prvCheckTask, "TaskName", DEFAULT_STACK_DEPTH, NULL, 1, &task);

  CHECK_EQUAL(pdPASS, ret);
  STRCMP_EQUAL("TaskName", pcTaskGetName(task));
}

TEST(xTaskCreate, ChangesCurrentTask)
{
  int ret = xTaskCreate(prvCheckTask, "TaskName", DEFAULT_STACK_DEPTH, NULL, 1, &task);

  CHECK_EQUAL(pdPASS, ret);
  CHECK_EQUAL(task, xTaskGetCurrentTaskHandle());
}

TEST(xTaskCreate, ChangesCurrentTaskName)
{
  int ret = xTaskCreate(prvCheckTask, "TaskName", DEFAULT_STACK_DEPTH, NULL, 1, &task);

  CHECK_EQUAL(pdPASS, ret);
  STRCMP_EQUAL("TaskName", pcTaskGetName(xTaskGetCurrentTaskHandle()));
  CHECK(checkCurrentTaskName("TaskName"));
}

TEST(xTaskCreate, SetStaticallyAllocated)
{
  int ret = xTaskCreate(prvCheckTask, "TaskName", DEFAULT_STACK_DEPTH, NULL, 1, &task);

  CHECK_EQUAL(pdPASS, ret);
  CHECK_TRUE(isDinamicallyAllocated());
}

int main(int argc, char* argv[])
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
