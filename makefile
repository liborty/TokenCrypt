CFLAGS = -O3 -I.

# DESTDIR is where the program should be installed
DESTDIR = /usr/local

all: symcrypt hexcheck hexify

symcrypt: crypt/src/main.rs
	sudo cargo install --path crypt --root ${DESTDIR}
	strip ${DESTDIR}/bin/symcrypt	
	cp ${DESTDIR}/bin/symcrypt .

#symcrypt: symcrypt.c
#	${CC} ${CFLAGS} $@.c -o $@
#	strip $@
#	chmod 755 $@
#	sudo cp $@ ${DESTDIR}

hexcheck: hex2bin/src/main.rs
	sudo cargo install --path hex2bin --root ${DESTDIR}
	strip ${DESTDIR}/bin/hexcheck	
	cp ${DESTDIR}/bin/hexcheck .

#hexcheck: hexcheck.c 
#	${CC} ${CFLAGS} $@.c -o $@
#	strip $@
#	chmod 755 $@
#	sudo cp $@ ${DESTDIR}
	
hexify: bin2hex/src/main.rs
	sudo cargo install --path bin2hex --root ${DESTDIR}
	strip ${DESTDIR}/bin/hexify
	cp ${DESTDIR}/bin/hexify .	

#hexify: hexify.c 
#	${CC} ${CFLAGS} $@.c -o $@
#	strip $@
#	chmod 755 $@
#	sudo cp $@ ${DESTDIR}
	
install: all
	sudo cp keygen ncrpt expcrypt impcrypt crptest expimptest ${DESTDIR}/bin
