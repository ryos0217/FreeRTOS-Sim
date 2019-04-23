#include "tasks.c"

int
checkCurrentTaskName (const char* const name)
{
  if ((name == NULL)
      || (strlen(name) == 0))
    return 0;

  return strncmp (name, pxCurrentTCB->pcTaskName, sizeof(pxCurrentTCB->pcTaskName)) == 0;
}
