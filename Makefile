CC		= gcc

NAME		= libmy_malloc_`uname`.so
SYMLINKNAME	= libmy_malloc.so

SRC		= my_malloc.c

OBJ		= $(SRC:%.c=%.o)

RM		= rm -f

CFLAGS		= -W -Wall -Werror -ansi -pedantic


.PHONY:			all dynamic_malloc clean fclean re

all:			dynamic_malloc


dynamic_malloc:		$(OBJ)
			$(CC) -c $(CFLAGS) -fpic $(SRC)
			$(CC) -shared -o $(NAME) $(OBJ)
			ln -s $(NAME) $(SYMLINKNAME)

clean:
			$(RM) $(OBJ)

fclean:			clean
			$(RM) $(NAME)
			$(RM) $(SYMLINKNAME)

re:			fclean all
