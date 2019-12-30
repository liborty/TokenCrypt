# TokenCrypt

**A simple utility to encrypt and decrypt multiple security tokens or any files.
High security without onerous complications.**

## Version 1.0.3  

## Outline

Internet security tokens are usually 32,64 or more hexadecimal characters. 
They are increasingly used to facilitate secure access to various 
Applications Programming Interfaces (APIs). Being plain text,
they are easily transmitted over the internet but they need to be stored securely,
that means encrypted.

The workhorse of TokenCrypt is `symcrypt.c` which does fast symmetric
encryption or decryption of any type of file of any length. It uses practically no memory.

Then there are 
bash helper scripts to generate keys, to automate the encryption and decryption
of whole directories and a testing script.

When a token consist of several parts, perhaps separated by a dash or some
other intervening non-hexadecimal characters, it is best to split it into individual
files containing only pure hexadecimal. Spaces and newlines will be tolerated
but will be deleted in the process.

The hexadecimal (token) files are automatically recognised and converted to binary, 
which halves them in length. Other files are used as they are.

Then compression is tried and if it results in a smaller file, its result is used.
(This will not generally be true for small and/or binary files).

Finally, the binary encryptor `symcrypt.c` is applied.
Decryption is the inverse of this process. See the scripts `ncrpt` and `dcrpt` for details.
However, the algorithm is transparent to the user, TokenCrypt can be used without
its knowledge.

The entire process of encryption and decryption can be automatically
tested using the script `test`.

It is possible and simpler to naively encrypt/decrypt the hexadecimal files/tokens
as they are but this is sub-optimal, as they are twice as long. It is probably 
pessimisation of security as well.

### General Encryption

There may be files and directories, perhaps even whole databases of security tokens,
all mixed up with other types of files. This is perhaps not the best practice but 
it may well arise and could involve much work to separate them.

TokenCrypt will nonetheless encrypt/decrypt whole directories, applying the best
procedure individually to each file.

## Installation

This software was tested under Linux. 
Installation from source needs just a C compiler, either clang or gcc.

Download or clone this directory, cd into it and then:  
**`sudo ./install`**  
Compiles and installs system-wide in /usr/local/bin. Uninstall everything with:  
**`sudo ./uninstall`**

Alternatively, if you are using a typical Linux, you can skip the compilation step
and use the pre-compiled binaries `symcrypt` and `hexcheck` included in this repository.
To do that, either comment out the compilation in the `install` script, or just move manually 
all the executables from here to your own bin directory that is in your path 
(this does not require `sudo` privileges).

### Dependencies

Standard utilities `xxd` and `lzma` which are normally pre-installed.

## Usage

**`ncrpt`** path/dirname

Fresh individual keys are generated for all the files in `path/dirname` and
written to the directory `./dirname_key` that mirrors the encrypted files
which go into `./dirname_crp`.

**`dcrpt`** path/dirname_key path/dirname_crp

is the inverse of `ncrpt`. Uses the keys from  `path/dirname_key` to decrypt 
their one-for-one corresponding files in `path/dirname_crp`.
Writes all the decrypted results into `./dirname_org`.

## Automated Testing (to be released soon)


**`test`** path/dirname 

Tests `ncrpt` and `dcrpt`. It first encrypts and then decrypts back again all the files in the given directory
and compares the results against the original files.
There should be no differences found, i.e. just a blank line.

If it lists any differences, then something has gone wrong. 
In that case the safe course of action is to delete the newly created `./dirname_key`,
`./dirname_crp` and `./dirname_org` directories.
The original files in `path/dirname` will be untouched.

## Conclusion

Watch this space, more facilities are in preparation.
