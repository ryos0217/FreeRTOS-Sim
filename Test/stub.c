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

 __attribute__((weak)) void
vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions )
{
}

#if( portUSING_MPU_WRAPPERS == 1 )
 __attribute__((weak)) StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters, BaseType_t xRunPrivileged ) PRIVILEGED_FUNCTION
{
  return 0;
}
#else
 __attribute__((weak)) StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters ) PRIVILEGED_FUNCTION
{
  return 0;
}
#endif

 __attribute__((weak)) unsigned long ulPortGetTimerValue( void )
{
  return 0;
}

 __attribute__((weak)) void vCoRoutineSchedule( void )
{
}
 __attribute__((weak)) void vApplicationIdleHook( void )
{
}
 __attribute__((weak)) void vApplicationTickHook( void )
{
}

 __attribute__((weak)) void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
}
 __attribute__((weak)) void vPortAddTaskHandle( void *pxTaskHandle )
{
}
 __attribute__((weak)) void vPortClearInterruptMask( portBASE_TYPE xMask )
{
}
 __attribute__((weak)) void vPortDisableInterrupts( void )
{
}
 __attribute__((weak)) void vPortEndScheduler( void ) PRIVILEGED_FUNCTION
{
}
 __attribute__((weak)) void vPortEnterCritical( void )
{
}
 __attribute__((weak)) void vPortExitCritical( void )
{
}
 __attribute__((weak)) void vPortFindTicksPerSecond( void )
{
}
 __attribute__((weak)) void vPortForciblyEndThread( void *pxTaskToDelete )
{
}
 __attribute__((weak)) void *pvPortMalloc( size_t xSize ) PRIVILEGED_FUNCTION
{
  if (malloc_fail)
    return NULL;

  if (xSize <= 0)
    return NULL;

  return malloc (xSize);
}
 __attribute__((weak)) void vPortFree( void *pv ) PRIVILEGED_FUNCTION
{
  if (!pv)
    return;

  free (pv);
}
 __attribute__((weak)) void vPortYield( void )
{
}

 __attribute__((weak)) BaseType_t xPortSetInterruptMask( void )
{
  return 0;
}
 __attribute__((weak)) BaseType_t xPortStartScheduler( void ) PRIVILEGED_FUNCTION
{
  return 0;
}
 __attribute__((weak)) BaseType_t xTimerCreateTimerTask( void ) PRIVILEGED_FUNCTION
{
  return 0;
}
