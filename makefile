CFLAGS = -O3 -I.

all: symcrypt hexcheck

symcrypt: symcrypt.c
	$(CC) symcrypt.c -o symcrypt
	strip symcrypt
	
hexcheck: hexcheck.c
	$(CC) hexcheck.c -o hexcheck
	strip hexcheck
	
