#include "FreeRTOS.h"
#include "list.h"
#include <stdint.h>

void *pvPortMalloc( size_t xWantedSize ){return NULL;}
void vPortFree( void *pv ){}
void vAssertCalled( unsigned long ulLine, const char * const pcFileName ){}
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters ){return NULL;}
void vPortEnterCritical( void ){}
void vPortAddTaskHandle( void *pxTaskHandle ){}
void vPortExitCritical( void ){}
void vPortYield( void ){}
void vPortForciblyEndThread( void *pxTaskToDelete ){}
portBASE_TYPE xPortSetInterruptMask( void ){return 0;}
void vPortClearInterruptMask( portBASE_TYPE xMask ){}
BaseType_t xTimerCreateTimerTask( void ){return 0;}
void vPortDisableInterrupts( void ){}
void vPortFindTicksPerSecond( void ){}
portBASE_TYPE xPortStartScheduler( void ){return 0;}
void vApplicationTickHook( void ){}
void vPortEndScheduler( void ){}
unsigned long ulPortGetTimerValue( void ){return 0;}
void vApplicationIdleHook( void ){}
void vListInitialiseItem( ListItem_t * const pxItem ){}
void vListInsertEnd( List_t * const pxList, ListItem_t * const pxNewListItem ){}
void vListInitialise( List_t * const pxList ){}
UBaseType_t uxListRemove( ListItem_t * const pxItemToRemove ){return 0;}
void vListInsert( List_t * const pxList, ListItem_t * const pxNewListItem ){}
