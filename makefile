CFLAGS = -O3 -I.

# DESTDIR is where the program should be installed
DESTDIR = /usr/local/bin

progs: symcrypt hexcheck hexify

symcrypt hexcheck hexify:
	${CC} ${CFLAGS} $@.c -o $@
	strip $@
	chmod 755 $@
	sudo cp $@ ${DESTDIR}
	
install: progs
	sudo cp keygen ncrpt dcrpt expcrypt impcrypt crptest expimptest ${DESTDIR}
