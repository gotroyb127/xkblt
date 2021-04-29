# xkblt

include config.mk

SRC = xkblt.c
OBJ = ${SRC:.c=.o}
PROG = xkblt

all: ${PROG}

.c.o:
	${CC} -c ${CFLAGS} $<

${PROG}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f ${OBJ} ${PROG}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${PROG} ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/${PROG}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${PROG}
