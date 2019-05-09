#include "tasks.c"

int checkCurrentTaskName(const char* const name)
{
  if ((name == NULL)
      || (strlen(name) == 0))
    return 0;

  return strncmp(name, pxCurrentTCB->pcTaskName, sizeof(pxCurrentTCB->pcTaskName)) == 0;
}

int isDinamicallyAllocated(void)
{

#if( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
  return pxCurrentTCB->ucStaticallyAllocated == tskDYNAMICALLY_ALLOCATED_STACK_AND_TCB;
#elif( configSUPPORT_STATIC_ALLOCATION == 1 )
  return 0;
#else
  return 1;
#endif
}
