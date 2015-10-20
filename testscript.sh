#!/bin/bash
#this script will invoker a new terminal with LD_PRELOAD set to our malloc

export LD_PRELOAD=`pwd`/libmy_malloc.so
/bin/bash
