/*
** test.c for my_malloc in /home/laxa/Documents/my_malloc
** 
** Made by EGLOFF Julien
** Login   <egloff_j@etna-alternance.net>
** 
** Started on  Thu Oct 22 17:54:35 2015 EGLOFF Julien
** Last update Thu Oct 22 17:54:37 2015 EGLOFF Julien
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

# define        PAGE_SIZE       4096
# define align8(x) (((x) + 7) & ~7)
# define pagealign(x)   (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

int     main(void)
{
  int   i;

  for (i = 0; i < 4098; i++)
  {
    printf("%d\n", pagealign(i));
  }
  return (0);
}
