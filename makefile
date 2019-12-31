CFLAGS = -O3

all: 
	$(CC) symcrypt.c -o symcrypt
	strip symcrypt
	$(CC) hexcheck.c -o hexcheck
	strip hexcheck
	
