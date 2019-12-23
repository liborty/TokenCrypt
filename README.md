# TokenCrypt

A simple utility to encrypt and decrypt multiple security tokens, or any files.
Tested under Linux. High security without onerous complications.

### Outline

Internet security tokens are usually 32,64 or more hexadecimal characters. 
They are increasingly used to facilitate secure access to various 
Applications Programming Interfaces (APIs) over the internet. Being plain text,
they are easily transmitted but they need to be stored securely, that means encrypted.

The heart of TokenCrypt is `symcrypt.c` which does fast symmetric 
encryption or decryption of any type of file of any length. 

Then there are a few 
bash helper scripts to generate keys, to convert .hex files to binary and back and
to automate the encryption of all files with .hex extension and the decryption of
all files with .scr extension (in the current directory).

If your token(s) consist of several parts, perhaps separated by a dash, 
it is best to split them into individual files, each containing only the hexadecimal characters.

In a Linux pipe, the hexadecimal token files (*.hex) are first converted to binary, 
which halves them in length. Then the binary encryptor `symcrypt.c` is applied.
Decryption is a simple inverse of this process. See `hexecrypt` and `hexdcrypt`.

It is possible to naively encrypt/decrypt directly the hexadecimal files/tokens
but this is sub-optimal, as the encrypted files are twice as long without any gains
in security. In fact, it is probably a pessimisation of security as well.

### symcrypt.c ###

Symcrypt is a general utility to encrypt/decrypt any files.
It is a stand-alone program written in C. It takes keyfile, infile and outfile as arguments.
Invoking `symcrypt` without any arguments gives you help information.

Outfile and infile can be omitted, in which case stdout and stdin are used. 
This form is particularly convenient for use in a Unix/Linux pipe. 
For the same reason, the program operates silently. Error messages are sent to stderr.

**Installation:** 

Download or clone this directory, cd into it and then:
```bash
clang (or gcc) symcrypt.c -o symcrypt  
chmod 700 symcrypt binkeygen hexecrypt hexdcrypt 
mv symcrypt binkeygen hexecrypt hexdcrypt some/bin/directory/in-your-path
```

**Operation:**

cd directory/with/your/hextokens 
 
Establish the largest or typical size of your token files (does not have to be exact) and generate a key of
that size, e.g.: 
```bash 
size=$( stat -c%s myfile.hex ); binkeygen $size > key.bin
```
Binkeygen is just a one-liner script: `</dev/urandom head -c $1`  
Hide the keyfile somewhere safe and secure but note its path as you will need it.
  
`hexecrypt keyfile` or just `hexecrypt`  
Normal usage is to supply the full path to your existing keyfile. 
Calling hexecrypt without an argument will automatically generate 
a new keyfile of a fixed predetermined length (8192 bits) and use it. 
As a precaution against overwriting an existing key, if key.bin is present in the
current directory, it will use that one instead of generating a new one.
The default length can be easily changed in the hexecrypt bash script.
  
In any case, all `*.hex` files will be encrypted into `*.scr` files and can 
now be deleted (at your own risk). It is strongly recommended that you test your
ability to use TokenCrypt safely and confidently, converting in both directions and
comparing to the original with `diff` before you start deleting any files. 
In particular, if you lose your keyfile, you will not be able to decrypt and thus any
deleted originals will be lost too.

`hexdcrypt keyfile`  
decrypts, recreating all the original `*.hex` files.


  




