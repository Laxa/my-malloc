/*
** my_malloc.c for my_malloc in /home/laxa/Documents/my_malloc
** 
** Made by EGLOFF Julien
** Login   <laxa>
** 
** Started on  Thu Oct 22 17:54:25 2015 EGLOFF Julien
** Last update Thu Nov 12 21:27:52 2015 EGLOFF Julien
*/

#include <unistd.h>
#include "my_malloc.h"

void *base = NULL;

void            *malloc(size_t size)
{
  t_block       *last;
  t_block       *b;
  size_t        s;

  if (size <= 0)
    return (NULL);
  s = ALIGN_8(size);
  if (base)
  {
    last = base;
    b = find_free_block(&last, s);
    if (b)
    {
      b->free = 0;
      split_chunk(b, b->size + sizeof(t_block), s);
    }
    else
      b = extend_heap(last, s);
  }
  else
  {
    b = extend_heap(NULL, s);
    base = b;
  }
  return (b != NULL ? ((void *)(((char *)b) + sizeof(t_block))) : (NULL));
}

void            free(void *ptr)
{
  t_block       *b;

  if ((void *)ptr > (void *)base && (void *)ptr < sbrk(0))
  {
    b = (t_block *)(((char *)ptr) - sizeof(t_block));
    b->free = 1;
    if (b->prev && b->prev->free)
      b = fusion(b->prev);
    if (b->next)
      fusion(b);
  }
}

void            *calloc(size_t nmemb, size_t size)
{
  size_t        *new;
  size_t        i;
  size_t        s;
  char          *data;

  if (size == 0 || nmemb == 0)
    return (NULL);
  new = malloc(nmemb * size);
  if (new)
  {
    s = ALIGN_8(nmemb * size);
    data = (char *)new;
    for (i = 0; i < s; i += size)
      data[i] = 0;
  }
  return (new);
}

void            *realloc(void *ptr, size_t size)
{
  size_t        *new;

  if (ptr == NULL)
    new = malloc(size);
  else
  {
    new = malloc(size);
    copy_data(ptr, new);
    free(ptr);
  }
  return (new);
}
