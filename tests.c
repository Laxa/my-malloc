#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int     main(void)
{
  void  *p;
  size_t        i;

  for (i = 0; i < 10000; i++)
  {
    p = malloc(i);
    free(p);
  }
  return (0);
}
