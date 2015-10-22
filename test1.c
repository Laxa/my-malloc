/*
** test1.c for my_malloc in /home/laxa/Documents/my_malloc
** 
** Made by EGLOFF Julien
** Login   <egloff_j@etna-alternance.net>
** 
** Started on  Thu Oct 22 17:54:44 2015 EGLOFF Julien
** Last update Thu Oct 22 17:54:46 2015 EGLOFF Julien
*/

#include <stdio.h>

int     main(void)
{
  char *test = (char *)malloc(300);
  char *test2 = (char *)malloc(300);
  char *test3 = (char *)malloc(300);

  memcpy(test, "\0", 300)
  return (0);
}
