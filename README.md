# TokenCrypt
**A simple utility to encrypt and decrypt multiple security tokens or any files.
High security without onerous complications.**

25th December 2019, © 2019 Libor Spacek

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

### Installation

This software was tested under Linux. Installation needs just a C compiler, either clang or gcc.

Download or clone this directory, cd into it and then: **`sudo ./install`**  
Compiles and installs system-wide in /usr/local/bin. 

Uninstall with: `sudo ./uninstall`

### Symcrypt

Symcrypt is a general utility to encrypt/decrypt any files.
It is a stand-alone program written in C. It takes keyfile, infile and outfile as arguments.
Invoking `symcrypt` without any arguments gives help information.

Outfile and infile can be omitted, in which case stdout and stdin are used. 
This form is particularly convenient for use in a Unix/Linux pipe. 
For the same reason, the program operates silently. Error messages are sent to stderr.

### Tokens Encryption

```bash
cd directory/with/your/hextokens
```

Establish the typical size of your token files (does not have to be exact) and generate a key of approximately half that size, e.g.: 
```bash 
size=$(( $(stat -c%s myfile.hex )/2 + 1 ))
binkeygen $size > key.bin
```
Binkeygen is just a one-liner script:  `</dev/urandom head -c $1` 
Hide the keyfile somewhere safe and secure (perhaps a USB pen?) and note its path as you will need it.
  
**`hexecrypt keyfile`**
  
The normal usage is to supply the full path to your existing keyfile.
 
All `*.hex` files in the current directory will be encrypted into `*.scr` files and
can then be manually deleted (at your own risk). It is strongly recommended 
that you test your ability to use this utility safely and confidently, 
converting in both directions and comparing to the original. 
Provided script `difftest` does all that automatically.
Be sure that `difftest` did not report any differences before you start deleting any files. 

In particular, if you lose your keyfile, you will not be able to decrypt and thus all
deleted originals will be lost too! This general peril of encryption can never 
be over emphasised. You have been warned.

**`hexecrypt`**

Calling hexecrypt without the keyfile argument will automatically generate 
new keyfile of a fixed predetermined length (currently 8192 bits) and use it.
The key length can be easily changed in the hexecrypt bash script.
When key.bin is already present in the current directory, it will be used instead.
This default behaviour is also a precaution against overwriting an existing key.

**`hexdcrypt keyfile`**

decrypts, recreating all the original `*.hex` files. 
Be careful that you do not overwrite your originals using a wrong key, 
save them first and delete the originals and the reconstructed versions 
only after a successful comparison test (difftest),
keeping just the encrypted versions.
It is probably a good idea to first backup your original tokens somewhere else as well.

**`difftest`**

Saves the existing key.bin and all `*.hex` files from the current directory to 
tests subdirectory, which it creates if necessary.

Then it encrypts and decrypts again all the `*.hex` files and compares the results
against the original files saved in tests directory. Finally, it reports how many files
had been tested. If it reports any differences, then something has gone wrong. 
In that case the safe course of action is to delete all `*.hex` and `*.scr` files from
the current directory and restore the original `*.hex` files from the tests directory.

### Final Remarks

All of the above programs will exit with an error message if files with 
relevant extensions are not present in the current directory. 

Similarly, when a keyfile is needed but is not specified or can not be found.
