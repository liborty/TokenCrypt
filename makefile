CFLAGS = -O3

symcrypt: symcrypt.c
	$(CC) symcrypt.c -o symcrypt
	strip symcrypt

hexcheck: hexcheck.c
	$(CC) hexcheck.c -o hexcheck
	strip hexcheck

all: symcrypt hexcheck 
