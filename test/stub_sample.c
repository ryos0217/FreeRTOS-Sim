#include "FreeRTOS.h"
#include "list.h"
#include <stdint.h>

void *pvPortMalloc( size_t xWantedSize ){}
void vPortFree( void *pv ){}
void vAssertCalled( unsigned long ulLine, const char * const pcFileName ){}
void vListInitialiseItem( ListItem_t * const pxItem ){}
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters ){}
void vPortEnterCritical( void ){}
void vPortAddTaskHandle( void *pxTaskHandle ){}
void vListInsertEnd( List_t * const pxList, ListItem_t * const pxNewListItem ){}
void vPortExitCritical( void ){}
void vPortYield( void ){}
UBaseType_t uxListRemove( ListItem_t * const pxItemToRemove ){return 0;}
void vPortForciblyEndThread( void *pxTaskToDelete ){}
portBASE_TYPE xPortSetInterruptMask( void ){return 0;}
void vPortClearInterruptMask( portBASE_TYPE xMask ){}
BaseType_t xTimerCreateTimerTask( void ){}
void vPortDisableInterrupts( void ){}
void vPortFindTicksPerSecond( void ){}
portBASE_TYPE xPortStartScheduler( void ){return 0;}
void vApplicationTickHook( void ){}
void vPortEndScheduler( void ){}
unsigned long ulPortGetTimerValue( void ){}
void vListInsert( List_t * const pxList, ListItem_t * const pxNewListItem ){}
void vApplicationIdleHook( void ){}
void vListInitialise( List_t * const pxList ){}
