/*
** test1.c for my_malloc in /home/laxa/Documents/my_malloc
** 
** Made by EGLOFF Julien
** Login   <egloff_j@etna-alternance.net>
** 
** Started on  Thu Oct 22 17:54:44 2015 EGLOFF Julien
** Last update Fri Nov  6 11:55:51 2015 EGLOFF Julien
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int     main(void)
{
  char *test = (char *)malloc(300);
  printf("test1 done\n");
  char *test2 = (char *)malloc(300);
  printf("test2 done\n");
  char *test3 = (char *)malloc(300);
  printf("test3 done\n");
  free(test);
  free(test2);
  free(test3);
  test = (char *)malloc(300);
  return (0);
}
