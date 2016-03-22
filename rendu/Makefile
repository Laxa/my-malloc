##
## Makefile for my_malloc in /home/laxa/Documents/my_malloc
## 
## Made by EGLOFF Julien
## Login   <laxa>
## 
## Started on  Thu Oct 22 17:54:58 2015 EGLOFF Julien
## Last update Thu Nov 12 21:27:21 2015 EGLOFF Julien
##

CC		= 	gcc

NAME		= 	libmy_malloc_`uname`.so
SYMLINKNAME	= 	libmy_malloc.so

SRC		= 	my_malloc_utils.c	\
			my_malloc.c

OBJ		= 	$(SRC:%.c=%.o)

RM		= 	rm -f

CFLAGS		= 	-W -Wall -Werror


.PHONY:			all dynamic_malloc clean fclean re test

all:			dynamic_malloc


dynamic_malloc:
			$(CC) -c $(CFLAGS) -I. -fpic $(SRC)
			$(CC) -shared -o $(NAME) $(OBJ)
			ln -fs $(NAME) $(SYMLINKNAME)

clean:
			$(RM) $(OBJ)

fclean:			clean
			$(RM) $(NAME)
			$(RM) $(SYMLINKNAME)

re:			fclean all
