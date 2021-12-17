CFLAGS = -O3 -I.

# DESTDIR is where the program should be installed
DESTDIR = /usr/local/bin

all: symcrypt hexcheck hexify

symcrypt: symcrypt.c
	${CC} ${CFLAGS} $@.c -o $@
	strip $@
	chmod 755 $@
	sudo cp $@ ${DESTDIR}

hexcheck: hexcheck.c 
	${CC} ${CFLAGS} $@.c -o $@
	strip $@
	chmod 755 $@
	sudo cp $@ ${DESTDIR}

hexify: hexify.c 
	${CC} ${CFLAGS} $@.c -o $@
	strip $@
	chmod 755 $@
	sudo cp $@ ${DESTDIR}
	
install: all
	sudo cp keygen ncrpt dcrpt expcrypt impcrypt crptest expimptest ${DESTDIR}
