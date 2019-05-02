#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <stdio.h>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"

static void
justEndSchedulerTask(void* pvParameters)
{
  int* ptr = (int*)pvParameters;
  *ptr = 0;
  vTaskEndScheduler();
}

void vApplicationMallocFailedHook(void)
{
  vAssertCalled(__LINE__, __FILE__);
}

void vApplicationTickHook(void)
{
}

void vApplicationIdleHook(void)
{
}

void vAssertCalled(unsigned long ulLine, const char* const pcFileName)
{
  taskENTER_CRITICAL();
  {
    fprintf(stderr, "[ASSERT] %s:%lu\n", pcFileName, ulLine);
  }
  taskEXIT_CRITICAL();
  exit(-1);
}
}

#define DEFAULT_STACK_DEPTH 2048

TEST_GROUP(Scheduler)
{
  int param;
  TaskHandle_t task;

  void setup()
  {
    param = -1;
    int ret = xTaskCreate(justEndSchedulerTask,
        "TaskName", DEFAULT_STACK_DEPTH,
        &param, 0, &task);

    CHECK_EQUAL(pdPASS, ret);
  }
  void teardown()
  {
    vTaskDelete(task);
  }
};

TEST(Scheduler, CallsOneTaskHandler)
{
  vTaskStartScheduler();

  CHECK_EQUAL(0, param);
}

int main(int argc, char* argv[])
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
