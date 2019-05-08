#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
#define PRIO_HIGH 3
#define PRIO_MID 2
#define PRIO_LOW 1
#define SIZE_TRACE 64

static TaskHandle_t taskTraceBuffer[SIZE_TRACE];
static void taskTraceInit()
{
  for (int i = 0; i < SIZE_TRACE; i++)
    taskTraceBuffer[i] = 0;
}

static void taskTrace()
{
  taskENTER_CRITICAL();

  for (int i = 0; i < SIZE_TRACE; i++) {
    if (taskTraceBuffer[i] == 0) {
      taskTraceBuffer[i] = xTaskGetCurrentTaskHandle();
#if 0
      fprintf(stderr, "@@@ %s: %d: taskTraceBuffer[%d] = %p\n",
          __FILE__, __LINE__,
          i, taskTraceBuffer[i]);
#endif
      break;
    }
  }

  taskEXIT_CRITICAL();
}

static int taskTraceCount()
{
  int count = 0;

  taskENTER_CRITICAL();

  for (int i = 0; i < SIZE_TRACE; i++) {
    if (taskTraceBuffer[i] == 0)
      break;

    ++count;
  }

  taskEXIT_CRITICAL();

  return count;
}

TEST_GROUP(Scheduler)
{
  void setup()
  {
    taskTraceInit();
    disableStdout();
  }
  void teardown()
  {
    restoreStdout();
  }

  private:
  int backupStdout;
  void disableStdout()
  {
    backupStdout = dup(fileno(stdout));
    if (backupStdout < 0)
      return;
    close(fileno(stdout));
    int nullFd = open("/dev/null", O_RDWR);
    CHECK_EQUAL(fileno(stdout), nullFd);
  }
  void restoreStdout()
  {
    dup2(backupStdout, fileno(stdout));
    close(backupStdout);
  }
};

static void
setParamZero(void* pvParameters)
{
  int* ptr = (int*)pvParameters;
  *ptr = 0;
  vTaskEndScheduler();
}

TEST(Scheduler, CallsHandlerWithParam)
{
  /* 1. SETUP */
  TaskHandle_t task;
  int param = -1;
  int ret = pdFAIL;
  ret = xTaskCreate(setParamZero,
      "task0",
      DEFAULT_STACK_DEPTH,
      &param, PRIO_LOW, &task);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(0, param);

  /* 4. CLEANUP */
  vTaskDelete(task);
}

static void
delayLoop(void* pvParameters)
{
  taskTrace();

  while (1)
    vTaskDelay(1);
}

TEST(Scheduler, CallsTwoDelayTask)
{
  /* 1. SETUP */
  TaskHandle_t tasks[2];
  int ret = pdFAIL;
  ret = xTaskCreate(delayLoop,
      "task0",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_LOW, &tasks[0]);
  CHECK_EQUAL(pdPASS, ret);
  ret = xTaskCreate(delayLoop,
      "task1",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_LOW, &tasks[1]);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(2, taskTraceCount());

  /* 4. CLEANUP */
  vTaskDelete(tasks[0]);
  vTaskDelete(tasks[1]);
}

static void
yieldLoop(void* pvParameters)
{
  taskTrace();

  if (taskTraceCount() == 2)
    vTaskEndScheduler();

  while (1)
    taskYIELD();
}

TEST(Scheduler, CallsTwoYieldTask)
{
  /* 1. SETUP */
  TaskHandle_t tasks[2];
  int ret = pdFAIL;
  ret = xTaskCreate(yieldLoop,
      "task0",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_LOW, &tasks[0]);
  CHECK_EQUAL(pdPASS, ret);
  ret = xTaskCreate(yieldLoop,
      "task1",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_LOW, &tasks[1]);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(2, taskTraceCount());

  /* 4. CLEANUP */
  vTaskDelete(tasks[0]);
  vTaskDelete(tasks[1]);
}

static void
busyLoop(void* pvParameters)
{
  taskTrace();

  if (taskTraceCount() == 2)
    vTaskEndScheduler();

  while (1)
    ;
}

TEST(Scheduler, CallsTwoBusyTaskTimeSlice)
{
  /* 1. SETUP */
  TaskHandle_t tasks[2];
  int ret = pdFAIL;
  ret = xTaskCreate(busyLoop,
      "task0",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_LOW, &tasks[0]);
  CHECK_EQUAL(pdPASS, ret);
  ret = xTaskCreate(busyLoop,
      "task1",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_LOW, &tasks[1]);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(2, taskTraceCount());

  /* 4. CLEANUP */
  vTaskDelete(tasks[0]);
  vTaskDelete(tasks[1]);
}

static void
storeIdTask(void* pvParameters)
{
  taskTrace();

  while (1)
    vTaskDelay(1);
}

TEST(Scheduler, CallsTwoDifferentPrioTasks)
{
  /* 1. SETUP */
  TaskHandle_t highTask, lowTask;
  int ret = pdFAIL;
  ret = xTaskCreate(storeIdTask,
      "highPrioTask",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_HIGH, &highTask);
  CHECK_EQUAL(pdPASS, ret);
  ret = xTaskCreate(storeIdTask,
      "lowPrioTask",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_LOW, &lowTask);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(highTask, taskTraceBuffer[0]);
  CHECK_EQUAL(lowTask, taskTraceBuffer[1]);

  /* 4. CLEANUP */
  vTaskDelete(highTask);
  vTaskDelete(lowTask);
}

static void
highPrioDelayedTask(void* pvParameters)
{
  taskTrace();

  vTaskDelay(10);

  taskTrace();

  vTaskEndScheduler();
}

static void
lowPrioBusyTask(void* pvParameters)
{
  taskTrace();

  while (1) {
    ;
  }
}

TEST(Scheduler, CallsHighPrioTaskPreemptively)
{
  /* 1. SETUP */
  TaskHandle_t highTask, lowTask;
  int ret = pdFAIL;
  ret = xTaskCreate(highPrioDelayedTask,
      "highPrioTask",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_HIGH, &highTask);
  CHECK_EQUAL(pdPASS, ret);
  ret = xTaskCreate(lowPrioBusyTask,
      "lowPrioTask",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_LOW, &lowTask);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(highTask, taskTraceBuffer[0]);
  CHECK_EQUAL(lowTask, taskTraceBuffer[1]);
  CHECK_EQUAL(highTask, taskTraceBuffer[2]);

  /* 4. CLEANUP */
  vTaskDelete(highTask);
  vTaskDelete(lowTask);
}

static void
highPrioNotifyTake(void* pvParameters)
{
  taskTrace();

  ulTaskNotifyTake(pdTRUE, -1);

  taskTrace();

  vTaskEndScheduler();
}

static void
lowPrioNotifyGive(void* pvParameters)
{
  TaskHandle_t* highTask = (TaskHandle_t*)pvParameters;

  taskTrace();

  xTaskNotifyGive(*highTask);

  taskTrace();

  while (1) {
    ;
  }
}

TEST(Scheduler, TaskNotificationOneWay)
{
  /* 1. SETUP */
  TaskHandle_t highTask, lowTask;
  int ret = pdFAIL;
  ret = xTaskCreate(highPrioNotifyTake,
      "highPrioTask",
      DEFAULT_STACK_DEPTH,
      NULL, PRIO_HIGH, &highTask);
  CHECK_EQUAL(pdPASS, ret);
  ret = xTaskCreate(lowPrioNotifyGive,
      "lowPrioTask",
      DEFAULT_STACK_DEPTH,
      &highTask, PRIO_LOW, &lowTask);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(highTask, taskTraceBuffer[0]);
  CHECK_EQUAL(lowTask, taskTraceBuffer[1]);
  CHECK_EQUAL(highTask, taskTraceBuffer[2]);

  /* 4. CLEANUP */
  vTaskDelete(highTask);
  vTaskDelete(lowTask);
}

static void
highPrioNotifyTakeAndGive(void* pvParameters)
{
  TaskHandle_t* lowTask = (TaskHandle_t*)pvParameters;

  taskTrace();

  ulTaskNotifyTake(pdTRUE, -1);

  taskTrace();

  xTaskNotifyGive(*lowTask);

  taskTrace();

  while (1)
    vTaskDelay(1);
}

static void
lowPrioNotifyGiveAndTake(void* pvParameters)
{
  TaskHandle_t* highTask = (TaskHandle_t*)pvParameters;

  taskTrace();

  xTaskNotifyGive(*highTask);

  taskTrace();

  ulTaskNotifyTake(pdTRUE, -1);

  taskTrace();

  vTaskEndScheduler();
}

TEST(Scheduler, TaskNotificationRoundTrip)
{
  /* 1. SETUP */
  TaskHandle_t highTask, lowTask;
  int ret = pdFAIL;
  ret = xTaskCreate(highPrioNotifyTakeAndGive,
      "highPrioTask",
      DEFAULT_STACK_DEPTH,
      &lowTask, PRIO_HIGH, &highTask);
  CHECK_EQUAL(pdPASS, ret);
  ret = xTaskCreate(lowPrioNotifyGiveAndTake,
      "lowPrioTask",
      DEFAULT_STACK_DEPTH,
      &highTask, PRIO_LOW, &lowTask);
  CHECK_EQUAL(pdPASS, ret);

  /* 2. EXEC */
  vTaskStartScheduler();

  /* 3. CHECK */
  CHECK_EQUAL(highTask, taskTraceBuffer[0]);
  CHECK_EQUAL(lowTask, taskTraceBuffer[1]);
  CHECK_EQUAL(highTask, taskTraceBuffer[2]);
  CHECK_EQUAL(lowTask, taskTraceBuffer[1]);
  CHECK_EQUAL(highTask, taskTraceBuffer[2]);

  /* 4. CLEANUP */
  vTaskDelete(highTask);
  vTaskDelete(lowTask);
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
