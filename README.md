# TokenCrypt

**A simple utility to encrypt and decrypt multiple security tokens or any files.
High security without onerous complications.**

## Version 1.0.2  

## Outline

Internet security tokens are usually 32,64 or more hexadecimal characters. 
They are increasingly used to facilitate secure access to various 
Applications Programming Interfaces (APIs). Being plain text,
they are easily transmitted over the internet but they need to be stored securely, that means encrypted.

The workhorse of TokenCrypt is `symcrypt.c` which does fast symmetric
encryption or decryption of any type of file of any length. It uses practically no memory.

Then there are 
bash helper scripts to generate keys, to convert .hex files to binary and back, 
to automate the encryption of all files with .hex extension, the decryption of
all files with .scr extension (in the current directory), compression and encryption of whole directories of any kind of files, their decryption and decompression and finally a couple of test scripts.

### Hexadecimal Encryption

When a token consist of several parts, perhaps separated by a dash or some other intervening non-hexadecimal characters, it is best split into individual files containing only pure hexadecimal.

The hexadecimal (token) files (*.hex) are first converted to binary, 
which halves them in length. Then the binary encryptor `symcrypt.c` is applied
(all within a single Linux pipe).
Decryption is the inverse of this process. See `hexecrypt` and `hexdcrypt`.

The entire process of hexadecimal encryption and decryption can be automatically tested using the script `hextest`.

It is possible to naively encrypt/decrypt directly the hexadecimal files/tokens
but this is sub-optimal, as the encrypted files are twice as long without any gains
in security. In fact, it is probably a pessimisation of security as well.

### General Encryption

There may be files, perhaps even whole databases of security tokens which mix up the 
hexadecimal data with other kinds of data. This is perhaps not the best practice but 
it may well arise and could involve much work to separate them.
In these circumstances, reach for `ecrypt` and `dcrypt` utilities below.
You may not get full 50% compression as with the specialised hex versions above but it will be more convenient.

Indeed, `ecrypt` and `dcrypt` are intended to be used on whole directories of all kinds of files and data, as general purpose archival programs. This process can be tested with the script ``dirtest``.

## Installation

This software was tested under Linux. 
Installation from source needs just a C compiler, either clang or gcc.

Download or clone this directory, cd into it and then:  
**`sudo ./install`**  
Compiles and installs system-wide in /usr/local/bin. Uninstall everything with:  
**`sudo ./uninstall`**

Alternatively, if you are using a typical Linux, you can skip the compilation step
and use the pre-compiled binary `symcrypt` included in this repository. To do that, 
either comment out the compilation in the install script, or just move manually 
all the executables from here to your own bin directory that is in your path 
(this does not require `sudo` privileges).

## Hexadecimal (Tokens) Encryption

`cd directory/with/your/hextokens-files`

`keygen mytypicalfile.hex > keyfile.key`

will generate a keyfile of approximately half the size of `mytypicalfile.hex`. Rename it, hide it somewhere safe and secure (perhaps a USB pen?) and note its path as you will need it. Back it up.
  
**`hexecrypt`** keyfile
  
The normal encryption/decryption usage is to supply the full path to an existing keyfile.
 
All `*.hex` files in the current directory will be encrypted into `*.scr` files and
can then be manually deleted (at your own risk). It is strongly recommended
that you test your ability to use this utility safely and confidently, by
converting in both directions and comparing to the original.
Script `hextest` is provided for convenient automated testing.
Be sure that `hextest` did not report any differences before you start deleting any original files.

Should you lose your keyfile, you will not be able to decrypt and thus any
deleted originals will be lost forever, too! This general peril of encryption can never
be over emphasised. You have been warned!

**`hexecrypt`**

Calling hexecrypt without a keyfile argument will automatically generate 
new keyfile of half the length of the first somefile.hex in the current directory and use it
for encryption.

When somefile.key is already present in the current directory, it will be used instead.
This default behaviour is also a precaution against overwriting an existing keyfile.

**`hexdcrypt`** keyfile

decrypts, recreating all the original `*.hex` files in the current directory. 
Be careful that you do not accidentally overwrite your originals using a wrong key!
Save them first. Delete the originals and the reconstructed versions 
only after a successful comparison test (hextest). Then you can keep just the encrypted versions. It is probably a good idea to first backup your original tokens somewhere else as well.

## General Encryption

**`ecrypt`** keyfile path/dirname

takes two arguments, the keyfile as above and a path/directory. 
When the keyfile is missing, or the directory does not exist, the script exits with error messages.
All files in the supplied directory will be compressed using `lzma`,
encrypted  using `symcrypt` and written to `./dirname_ecr`

**`dcrypt`** keyfile path/dirname

is the inverse of `ecrypt`. The restored (copies of) the original files are
written into `./dirname_org`

**`superecrypt`** path/dirname

Super security version of `ecrypt` above. Omits the keyfile argument. Instead, fresh keys are generated for all the files in `path/dirname` and written to the directory `./dirname_key` that mirrors the encrypted files in `./dirname_ecr` and `./dirname_elz`.
The last directory is used selectively only for files which are actually shortened by lzma compression, which is generally not true for short and/or binary files.

**`superdcrypt`** path/dirname-key path/dirname_ecr path/dirname-ecrlz

is the inverse of `superecrypt`. Uses the keys from  `path/dirname_key` to decrypt their one-for-one corresponding files found either in `path/dirname_ecr` or in `path/dirname_elz` and writes all the decrypted results into `./dirname_org`

**`symcrypt`**

is a general utility to encrypt/decrypt any files.
It is a stand-alone program written in C. It takes keyfile, infile and outfile as arguments.
Invoking `symcrypt` without any arguments gives help information.

Outfile and infile can be omitted, in which case stdout and stdin are used.
This form is particularly convenient for use in a Unix/Linux pipe.
For the same reason, the program operates silently. Error messages are sent to stderr.

Symcrypt is invoked by most of the TokenCrypt utilities to
carry out the actual encryption and decryption.

## Testing

**`hextest`**

Tests hexadecimal encryption and decryption: `hexecrypt` and `hexdcrypt`.

Generates new keyfile, just for this test.

Saves all `*.hex` files from the current directory to 
`./tests` subdirectory, which it creates if necessary.

Then it first encrypts and then decrypts back again all the `*.hex` 
files in the current directory and compares the results against the original files saved in the tests directory.
Finally, `hextest` reports how many files had been tested. 
If it lists any differences, then something has gone wrong. 
In that case the safe course of action is to delete all `*.hex` and `*.scr` files from
the current directory and restore the saved original `*.hex` files from the tests directory.

**`dirtest`** path/dirname

Tests general encryption and decryption: `ecrypt` and `dcrypt`.

Generates new keyfile, just for this test.

Encrypts and then decrypts back all the files in a given directory (normally not the current one).
`Ecrypt` creates `./dirname_ecr` under the current directory with all the compressed files in it. Then `dcrypt` creates `./dirname_ecr_org`. 

This script then compares all the original files in `path/dirname` against the newly recreated ones in 
`./dirname_ecr_org` and lists any differences. There should be none, i.e. just a blank line.

**`supertest`** path/dirname

Similar to `dirtest` above. Tests general encryption and decryption using `superecrypt` and `superdecrypt`. Creates three output directories in the current directory and deletes them afterwards.

## Conclusion

Watch this space, more facilities are in preparation.

The next release (1.0.3) will feature `hexcheck` to analyse file contents
to automatically detect entirely hexadecimal files. This will bring two important improvements:

 1. not having to rely on extension names

 2. considerable simplification of usage, as appropriate scripts will be 
 applied automatically
