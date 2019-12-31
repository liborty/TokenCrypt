CFLAGS = -O3

all: symcrypt.c hexcheck.c
	$(CC) symcrypt.c -o symcrypt
	strip symcrypt
	$(CC) hexcheck.c -o hexcheck
	strip hexcheck
	
