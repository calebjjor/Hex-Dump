#
#  Project 1 of COMP389: Cryptography
#  Function: Makefile
#  File Name: Makefile
#

CC = gcc
CFLAGS = -Wall
RM = rm -f
TAR = gtar czvf proj1.tar.gz
SOURCE = proj1.c fns.c fns.h
SUPPORT = Makefile README
OBJS=proj1.o fns.o

default: all

all: proj1

proj1: $(OBJS)
	$(CC) -o proj1 $(CFLAGS) $(OBJS)

fns.o: fns.h

clean: 
	$(RM) *.o proj1 *~

tar: 
	$(TAR) $(SOURCE) $(SUPPORT)

