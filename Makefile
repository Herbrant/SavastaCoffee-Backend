# Define required macros here
SHELL = /bin/sh

OBJS =  app
CFLAG = -Wall
CC = gcc
INCLUDES = main.c handlers.c database.c
LIBS = -l ulfius -l jansson -l sqlite3 -l crypt

build:${OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o ${OBJS} ${LIBS}

clean:
	-rm -f *.o app