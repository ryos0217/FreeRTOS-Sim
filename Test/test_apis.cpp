#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <string.h>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"

void vApplicationMallocFailedHook(void)
{
  vAssertCalled(__LINE__, __FILE__);
}

void vApplicationTickHook(void)
{
}

void vApplicationIdleHook(void)
{
  vTaskEndScheduler();
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

TEST_GROUP(Scheduler){
  void setup(){} void teardown(){}
};

static void
justEndSchedulerTask(void* pvParameters)
{
  int* ptr = (int*)pvParameters;
  *ptr = 0;
  vTaskEndScheduler();
}

TEST(Scheduler, CallsOneTaskHandler)
{
  /* 1. SETUP */
  TaskHandle_t task;
  int param = -1;
  int ret = pdFAIL;
  ret = xTaskCreate(justEndSchedulerTask,
      "JustEndSchedulerTask",
      DEFAULT_STACK_DEPTH,
      &param, 1, &task);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(0, param);

  /* 4. CLEANUP */
  vTaskDelete(task);
}

static void
countUpTask(void* pvParameters)
{
  int* ptr = (int*)pvParameters;

  taskENTER_CRITICAL();

  int count = *ptr;
  ++count;
  *ptr = count;

  taskEXIT_CRITICAL();

  while (1) {
    vTaskDelay(1);
  }
}

TEST(Scheduler, CallsTwoTaskHandlers)
{
  /* 1. SETUP */
  TaskHandle_t tasks[2];
  int count = 0;
  int ret = pdFAIL;
  ret = xTaskCreate(countUpTask,
      "countUpTask0",
      DEFAULT_STACK_DEPTH,
      &count, 1, &tasks[0]);
  CHECK_EQUAL(pdPASS, ret);
  ret = xTaskCreate(countUpTask,
      "countUpTask1",
      DEFAULT_STACK_DEPTH,
      &count, 1, &tasks[1]);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(2, count);

  /* 4. CLEANUP */
  vTaskDelete(tasks[0]);
  vTaskDelete(tasks[1]);
}

static void
countUpBusyTask(void* pvParameters)
{
  int* ptr = (int*)pvParameters;

  taskENTER_CRITICAL();

  int count = *ptr;
  ++count;
  *ptr = count;

  taskEXIT_CRITICAL();

  if (count == 2) {
    vTaskEndScheduler();
  } else {
    while (1)
      taskYIELD();
  }
}

TEST(Scheduler, CallsTwoBusyTaskHandlers)
{
  /* 1. SETUP */
  TaskHandle_t tasks[2];
  int count = 0;
  int ret = pdFAIL;
  ret = xTaskCreate(countUpBusyTask,
      "countUpTask0",
      DEFAULT_STACK_DEPTH,
      &count, 1, &tasks[0]);
  CHECK_EQUAL(pdPASS, ret);
  ret = xTaskCreate(countUpBusyTask,
      "countUpTask1",
      DEFAULT_STACK_DEPTH,
      &count, 1, &tasks[1]);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(2, count);

  /* 4. CLEANUP */
  vTaskDelete(tasks[0]);
  vTaskDelete(tasks[1]);
}

static void
storeIdTask(void* pvParameters)
{
  TaskHandle_t* history = (TaskHandle_t*)pvParameters;
  TaskHandle_t handle = xTaskGetCurrentTaskHandle();

  taskENTER_CRITICAL();

  for (int i = 0; i < 1024; i++) {
    if (history[i] == 0) {
      history[i] = handle;
      break;
    }
  }

  taskEXIT_CRITICAL();

  while (1)
    vTaskDelay(1);
}

TEST(Scheduler, CallsTwoDifferentPrioTasks)
{
  /* 1. SETUP */
  TaskHandle_t tasks[2];
  TaskHandle_t history[1024] = { 0 };
  int ret = pdFAIL;
  ret = xTaskCreate(storeIdTask,
      "highPrioTask",
      DEFAULT_STACK_DEPTH,
      history, 2, &tasks[0]);
  CHECK_EQUAL(pdPASS, ret);
  ret = xTaskCreate(storeIdTask,
      "lowPrioTask",
      DEFAULT_STACK_DEPTH,
      history, 1, &tasks[1]);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(history[0], tasks[0]);
  CHECK_EQUAL(history[1], tasks[1]);

  /* 4. CLEANUP */
  vTaskDelete(tasks[0]);
  vTaskDelete(tasks[1]);
}

int main(int argc, char* argv[])
{
  /* Enable -p option to run tests in a separate process.
     To call pthread_once for each test. */
  int new_argc = argc + 1;
  char** new_argv = (char**)malloc(sizeof(char*) * new_argc);
  for (int i = 0; i < argc; i++)
    new_argv[i] = strdup(argv[i]);
  new_argv[argc] = strdup("-p");
  return CommandLineTestRunner::RunAllTests(new_argc, new_argv);
}
