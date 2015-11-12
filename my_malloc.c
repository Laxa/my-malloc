/*
** my_malloc.c for my_malloc in /home/laxa/Documents/my_malloc
** 
** Made by EGLOFF Julien
** Login   <egloff_j@etna-alternance.net>
** 
** Started on  Thu Oct 22 17:54:25 2015 EGLOFF Julien
** Last update Thu Nov 12 21:12:25 2015 EGLOFF Julien
*/

#include <unistd.h>
#include <stdio.h>
#include "my_malloc.h"

static void *base = NULL;

void            *malloc(size_t size)
{
  t_block       *last;
  t_block       *b;
  size_t        s;

  printf("malloc called with: %lu size, base is 0x%08x\n", size, base);
  debug_info();
  if (size == 0)
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
    if (b == NULL)
      return (NULL);
  }
  else
  {
    b = extend_heap(NULL, s);
    if (b == NULL)
      return (NULL);
    base = b;
  }
  debug_info();
  printf("base is 0x%08x, returning 0x%08x\n", base, ((char *)b) + sizeof(t_block));
  return ((void *)(((char *)b) + sizeof(t_block)));
}

void            free(void *ptr)
{
  t_block       *b;

  if (ptr == NULL)
    puts("sending null pointer to free");
  else
  {
    printf("free called with 0x%08x\n", ptr);
    debug_info();
  }
  if ((void *)ptr > (void *)base && (void *)ptr < sbrk(0))
  {
    b = (t_block *)(((char *)ptr) - sizeof(t_block));
    if (b->magic != MAGIC)
    {
      puts("Memory corruption detected");
      return;
    }
    b->free = 1;
    if (b->prev && b->prev->free)
      b = fusion(b->prev);
    if (b->next)
      fusion(b);
    /* if (!b->next && sbrk(0) - (void *)b >= PAGE_SIZE) */
    /* { */
    /*   if (b->prev) */
    /*     b->prev->next = NULL; */
    /*   else */
    /*     base = NULL; */
    /*   brk(b); */
    /* } */
  }
  else
    puts("free pointer isn't valid");
  debug_info();
  puts("free ended");
}

void            *calloc(size_t nmemb, size_t size)
{
  size_t        *new;
  size_t        i;
  size_t        s;
  char          *data;

  printf("calloc called with %lu nmemb and %lu size\n", nmemb, size);
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
  printf("called returning 0x%08x\n", new);
  return (new);
}

void            *realloc(void *ptr, size_t size)
{
  size_t        *new;

  printf("realloc called with 0x%08x ptr and %lu size\n", ptr, size);
  if (ptr == NULL)
    new = malloc(size);
  else
  {
    new = malloc(size);
    copy_data(ptr, new);
    free(ptr);
  }
  printf("realloc returning %x\n", new);
  return (new);
}

static void     copy_data(void *src, size_t *dest)
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

static t_block  *fusion(t_block *b)
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
  split_chunk(b, aligned, size);
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

static void     *find_free_block(t_block **last, size_t size)
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

__attribute__ ((unused)) static void     debug_info(void)
{
  t_block       *b;
  size_t        free_mem = 0;
  int           free = 0;

  b = base;
  while (b)
  {
    printf("chunk 0x%08x,size:%lu,free:%d,magic:%d,next:0x%08x,prev:0x%08x\n", b, b->size, b->free, b->magic, b->next, b->prev);
    if (b->magic != 42)
      puts("/!\\ MEMORY CORRUPTION DETECTED /!\\");
    free_mem += b->size;
    b = b->next;
    free++;
  }
  printf("sbrk(0):0x%08x\n", sbrk(0));
}
