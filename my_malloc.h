/*
** my_malloc.h for my_malloc in /home/laxa/Documents/my_malloc
** 
** Made by EGLOFF Julien
** Login   <laxa>
** 
** Started on  Thu Oct 22 17:55:06 2015 EGLOFF Julien
** Last update Wed Nov 11 21:56:39 2015 EGLOFF Julien
*/

#ifndef __MY_MALLOC_H__
# define __MY_MALLOC_H__

# define        PAGE_SIZE       4096
# define        MAGIC           42

typedef struct s_block t_block;

# define ALIGN_8(x)      (((x) + 7) & ~7)
# define PAGE_ALIGN(x)   (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

typedef struct  s_block
{
  size_t        size;
  int           free;
  int           magic;
  t_block       *next;
  t_block       *prev;
}               t_block;

void            *malloc(size_t size);
void            free(void *ptr);
void            *calloc(size_t nmemb, size_t size);
void            *realloc(void *ptr, size_t);
static void     copy_data(void *src, size_t *dest);
static t_block  *fusion(t_block *b);
static void     *extend_heap(t_block *last, size_t size);
static void     *split_chunk(t_block *b, size_t chunk_size, size_t size);
static void     *find_free_block(t_block **last, size_t size);
static void     debug_info(void);

#endif /* !__MY_MALLOC_H__ */
