CFLAGS = -O3 -I.

# DESTDIR is where the program should be installed
DESTDIR = /usr/local/bin

progs: symcrypt hexcheck hexify
	strip $<
	chmod 755 $<
	sudo cp $< ${DESTDIR}

scripts: keygen ncrpt dcrpt crptest
	chmod 755 $<
	sudo cp $< ${DESTDIR}

all: progs scripts
