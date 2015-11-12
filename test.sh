#!/bin/bash

make re
gcc -g -W -Wall -Werror test1.c
export LD_PRELOAD=./libmy_malloc_Linux.so
./a.out
unset LD_PRELOAD
