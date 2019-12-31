CFLAGS = -O3

all: symcrypt.c hexcheck.c 

symcrypt.c: 
	$(CC) symcrypt.c -o symcrypt
	strip symcrypt 

hexcheck.c:
	$(CC) hexcheck.c -o hexcheck
	strip hexcheck
