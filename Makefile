SRC = lua_utils.c main.c
OBJ = ${SRC:.c=.o}

CFLAGS = -O3 -flto -I./include -I/usr/include -I/usr/local/include -std=gnu11
LDFLAGS = -L/usr/lib -L/usr/local/lib -L/usr/lib/lua/5.4 -llua -lm -ldl -lpthread

all: options corg

options:
	@echo corg build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	 ${CC} -c ${CFLAGS} $<

corg: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f corg ${OBJ}

CC = gcc

.PHONY: all options clean
