/*
** my_malloc.c for my_malloc in /home/laxa/Documents/my_malloc
** 
** Made by EGLOFF Julien
** Login   <egloff_j@etna-alternance.net>
** 
** Started on  Thu Oct 22 17:54:25 2015 EGLOFF Julien
** Last update Thu Oct 22 18:25:04 2015 EGLOFF Julien
*/

#include <unistd.h>
#include <stdio.h>
#include "my_malloc.h"

static t_block *base = NULL;

/* malloc algorythm : */
/* if size == 0 we return null */
/* we get size + block_size align to 8 */
/* we search a block free to contain that size */
/* if we find one, we use it and append metadata block at the end of it */
/* if not, we call sbrk to increase our heap by PAGE_SIZE multiplier */
void            *malloc(size_t size)
{
  t_block       *last;
  t_block       *b;
  size_t        s;

  if (size == 0)
    return NULL;
  s = ALIGN_8(size);
  if (base)
  {
    last = base;
    b = find_block(last, s);
    if (b)
    {

    }
    else
    {

    }
  }
  else
  {
    b = extend_heap(NULL, s);
    if (b == NULL)
      return NULL;
    base = b;
  }
  return (b + sizeof(t_block));
}

/* free algorythm : */
/* if ptr is valid : */
/*   - we get block address */
/*   - we mark it free */
/*   - if prev is free too, we fusion all previously free blocks */
/*   - if next is free, we do the same */
/*   - if we are at last block, we release memory till that point using brk */
/*   - if there is no more block, we reset the malloc state */
void    free(__attribute__ ((unused)) void *ptr)
{
  if (ptr > base && ptr < sbrk(0))
  {
    /* verify magic here */
  }
}

static void     *extend_heap(t_block *last, size_t size)
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
  b = split_chunk(b, aligned, size);
  return (b);
}

static void     *split_chunk(t_block *b, size_t chunk_size, size_t size)
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
    new = b + sizeof(t_block) + b->size;
    new->size = free - sizeof(t_block);
    new->free = 1;
    new->magic = MAGIC;
    new->prev = b;
    new->next = NULL;
    b->next = new;
  }
  return (new);
}
