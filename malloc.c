#include <unistd.h>

#define BLOCK_SIZE 37
typedef struct s_block *t_block;

static t_block find_block (t_block *last , size_t size);
static t_block extend_heap (t_block last , size_t s);
static void split_block(t_block b, size_t s);

/* block struct */
struct s_block {
  size_t size;
  struct s_block *next;
  struct s_block *prev;
  int free;
  void *ptr;
  /* A pointer to the allocated block */
  char data [1];
};

#define align4(x) (((x) + 7) & ~7)

void *base=NULL;

void *malloc(size_t size ){
  t_block b,last;
  size_t s;
  s = align4(size);
  if (base) {
    /* First find a block */
    last = base;
    b = find_block (&last ,s);
    if (b) {
      /* can we split */
      if ((b->size - s) >= ( BLOCK_SIZE + 4))
        split_block (b,s);
      b->free =0;
    } else {
      /* No fitting block , extend the heap */
      b = extend_heap (last ,s);
      if (!b)
        return(NULL );
    }
  } else {
    /* first time */
    b = extend_heap (NULL ,s);
    if (!b)
      return(NULL );
    base = b;
  }
  return(b->data );
}

t_block find_block (t_block *last , size_t size ){
  t_block b=base;
  while (b && !(b->free && b->size >= size )) {
    *last = b;
    b = b->next;
  }
  return (b);
}

void *calloc(size_t number , size_t size ){
  size_t *new;
  size_t s4 ,i;
  new = malloc(number * size );
  if (new) {
    s4 = align4(number * size) << 2;
    for (i=0; i<s4 ; i++)
      new[i] = 0;
  }
  return (new );
}
t_block fusion(t_block b){
  if (b->next && b->next ->free ){
    b->size += BLOCK_SIZE + b->next ->size;
    b->next = b->next ->next;
    if (b->next)
      b->next ->prev = b;
  }
  return (b);
}


typedef struct s_block *t_block;

/* Get the block from and addr */
t_block get_block (void *p)

{
  char *tmp;
  tmp = p;
  return (p = tmp -= BLOCK_SIZE );
}

/* Valid addr for free */
int valid_addr (void *p)
{
  if (base)
  {
    if ( p>base && p<sbrk (0))
    {
      return (p == ( get_block (p))->ptr );
    }
  }
  return (0);
}

/* The free */
/* See free(3) */
void free(void *p)
{
  t_block b;
  if ( valid_addr (p))
  {
    b = get_block (p);
    b->free = 1;
    /* fusion with previous if possible */
    if(b->prev && b->prev ->free)
      b = fusion(b->prev );
    /* then fusion with next */
    if (b->next)
      fusion(b);
    else
    {
      /* free the end of the heap */
      if (b->prev)
        b->prev ->next = NULL;
      else
        /* No more block !*/
        base = NULL;
      brk(b);
    }
  }
}
/* Copy data from block to block */
void copy_block (t_block src , t_block dst)
{
  int *sdata ,* ddata;
  size_t i;
  sdata = src ->ptr;
  ddata = dst ->ptr;
  for (i=0; i*4<src ->size && i*4<dst ->size; i++)
    ddata[i] = sdata[i];
}

/* The realloc */
/* See realloc(3) */
void *realloc(void *p, size_t size)
{
  size_t s;
  t_block b, new;
  void *newp;
  if (!p)
    return (malloc(size ));
  if ( valid_addr (p))
  {
    s = align4(size );
    b = get_block (p);
    if (b->size >= s)
    {
      if (b->size - s >= ( BLOCK_SIZE + 4))
        split_block (b,s);
    }
    else
    {
      /* Try fusion with next if possible */
      if (b->next && b->next ->free
          && (b->size + BLOCK_SIZE + b->next ->size) >= s)
      {
        fusion(b);
        if (b->size - s >= ( BLOCK_SIZE + 4))
          split_block (b,s);
      }
      else
      {
        /* good old realloc with a new block */
        newp = malloc(s);
        if (! newp)
          /* we’re doomed ! */
          return (NULL );
        /* I assume this work ! */
        new = get_block (newp );
        /* Copy data */
        copy_block (b,new );
        /* free the old one */
        free(p);
        return (newp );
      }
    }
    return (p);
  }
  return (NULL );
}

/* Split block according to size. */
/* The b block must exist. */
void split_block (t_block b, size_t s)
{
  t_block new;
  new = (t_block )(b->data + s);
  new ->size = b->size - s - BLOCK_SIZE ;
  new ->next = b->next;
  new ->prev = b;
  new ->free = 1;
  new ->ptr = new ->data;
  b->size = s;
  b->next = new;
  if (new ->next)
    new ->next ->prev = new;
}

/* Add a new block at the of heap */
/* return NULL if things go wrong */
t_block extend_heap (t_block last , size_t s)
{
  int sb;
  t_block b;
  b = sbrk (0);
  sb = (int)sbrk( BLOCK_SIZE + s);
  if (sb < 0)
    return (NULL );
  b->size = s;
  b->next = NULL;
  b->prev = last;
  b->ptr = b->data;
  if (last)
    last ->next = b;
  b->free = 0;
  return (b);
}
