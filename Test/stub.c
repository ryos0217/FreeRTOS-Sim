#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include "projdefs.h"
#include "portable.h"
#include "FreeRTOS.h"
#include "list.h"

static int malloc_fail = 0;

void
stub_reset (void)
{
  malloc_fail = 0;
}

void
stub_set_malloc_fail (void)
{
  malloc_fail = 1;
}

void
vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions )
{
}

#if( portUSING_MPU_WRAPPERS == 1 )
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters, BaseType_t xRunPrivileged ) PRIVILEGED_FUNCTION
{
  return 0;
}
#else
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters ) PRIVILEGED_FUNCTION
{
  return 0;
}
#endif

unsigned long ulPortGetTimerValue( void )
{
  return 0;
}

void vCoRoutineSchedule( void )
{
}
void vApplicationIdleHook( void )
{
}
void vApplicationTickHook( void )
{
}

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
}
void vPortAddTaskHandle( void *pxTaskHandle )
{
}
void vPortClearInterruptMask( portBASE_TYPE xMask )
{
}
void vPortDisableInterrupts( void )
{
}
void vPortEndScheduler( void ) PRIVILEGED_FUNCTION
{
}
void vPortEnterCritical( void )
{
}
void vPortExitCritical( void )
{
}
void vPortFindTicksPerSecond( void )
{
}
void vPortForciblyEndThread( void *pxTaskToDelete )
{
}
void *pvPortMalloc( size_t xSize ) PRIVILEGED_FUNCTION
{
  if (malloc_fail)
    return NULL;

  if (xSize <= 0)
    return NULL;

  return malloc (xSize);
}
void vPortFree( void *pv ) PRIVILEGED_FUNCTION
{
  if (!pv)
    return;

  free (pv);
}
void vPortYield( void )
{
}

BaseType_t xPortSetInterruptMask( void )
{
  return 0;
}
BaseType_t xPortStartScheduler( void ) PRIVILEGED_FUNCTION
{
  return 0;
}
BaseType_t xTimerCreateTimerTask( void ) PRIVILEGED_FUNCTION
{
  return 0;
}
