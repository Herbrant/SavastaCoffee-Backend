# Define required macros here
SHELL = /bin/sh

OBJS =  app
CFLAG = -Wall
CC = gcc
INCLUDES = main.c handlers.c
LIBS = -l ulfius -l jansson

build:${OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o ${OBJS} ${LIBS}

clean:
	-rm -f *.o app