#include <unistd.h>
#include "my_malloc.h"

void    *malloc(size_t size)
{
  return ((void *)size);
}

void    free(void *ptr)
{
  void *tmp = ptr;
  ptr = tmp;
  tmp = NULL;
}
