/*
** my_malloc_utils.c for my_malloc in /home/laxa/Rendu/egloff_j
** 
** Made by EGLOFF Julien
** Login   <laxa>
** 
** Started on  Thu Nov 12 21:25:12 2015 EGLOFF Julien
** Last update Thu Nov 12 21:34:13 2015 EGLOFF Julien
*/

#include <unistd.h>
#include "my_malloc.h"

void     copy_data(void *src, size_t *dest)
{
  size_t        i;
  t_block       *old;
  t_block       *new;
  char          *sdata;
  char          *ddata;

  sdata = (char *)src;
  ddata = (char *)dest;
  old = (t_block *)(((char *)src) - sizeof(t_block));
  new = (t_block *)(((char *)dest) - sizeof(t_block));
  for (i = 0; i < old->size && i < new->size; i++)
    ddata[i] = sdata[i];
}

t_block  *fusion(t_block *b)
{
  if (b->next && b->next->free)
  {
    b->size += sizeof(t_block) + b->next->size;
    b->next = b->next->next;
    if (b->next)
      b->next->prev = b;
  }
  return (b);
}

void     *extend_heap(t_block *last, size_t size)
{
  t_block       *b;
  size_t        aligned;

  aligned = PAGE_ALIGN(size + sizeof(t_block));
  b = sbrk(0);
  if (sbrk(aligned) == (void *) -1)
      return (NULL);
  b->size = size;
  b->prev = last;
  b->magic = MAGIC;
  b->next = NULL;
  if (last)
    last->next = b;
  b->free = 0;
  split_chunk(b, aligned, size);
  return (b);
}

void     *split_chunk(t_block *b, size_t chunk_size, size_t size)
{
  t_block       *new;
  size_t        free;

  free = chunk_size - size - sizeof(t_block);
  if (free < ALIGN_8(sizeof(t_block) + 1))
  {
    b->size += free;
    return (b);
  }
  else
  {
    b->size = size;
    new = (t_block *)(((char *)b) + sizeof(t_block) + size);
    new->size = free - sizeof(t_block);
    new->free = 1;
    new->magic = MAGIC;
    new->prev = b;
    new->next = NULL;
    b->next = new;
  }
  return (new);
}

void     *find_free_block(t_block **last, size_t size)
{
  t_block       *b;

  b = base;
  while (b && !(b->free && b->size >= (size + sizeof(t_block))))
  {
    *last = b;
    b = b->next;
  }
  return (b);
}
