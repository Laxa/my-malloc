/*
** test2.c for my_malloc in /home/laxa/Documents/my_malloc
** 
** Made by EGLOFF Julien
** Login   <laxa>
** 
** Started on  Thu Oct 22 17:54:52 2015 EGLOFF Julien
** Last update Thu Oct 22 17:54:54 2015 EGLOFF Julien
*/

#include <unistd.h>
#include <stdio.h>

int     main(void)
{
  void  *p = sbrk(0);
  printf("0x%016x\n", p);
  return (0);
}
