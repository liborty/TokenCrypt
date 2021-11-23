# TokenCrypt [<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/liborty/tokencrypt/HEAD?logo=github">](https://github.com/liborty/tokencrypt) [![Actions Status](https://github.com/liborty/TokenCrypt/workflows/test/badge.svg)](https://github.com/liborty/TokenCrypt/actions) 

**Efficiently compress and securely encrypt directories containing  hexadecimal security tokens, base64 data, text, binary and any other types of files. High security with ease of use.**

**Disclaimer:** use at your own risk! No warranties are given or implied. By downloading this software, you agree not to use it for unethical purposes.

**Warning:** the encryption keys and the encrypted files are written to two separate subdirectories in the current directory. Should you subsequently lose track of the association between them or lose the keys entirely, it will not be possible to reconstruct your original data! When encrypting many directories, take care to exactly mirror their structure with the generated key directories.

## Introduction

`Ncrpt` reads whole directories containing API tokens, base64 files and
any other types of files. It recognises hexadecimal files and base64 files and converts them to compact binary data. It then selects the best compression method for each file and finally encrypts them all with high security.

Internet security tokens usually consist of 32, 64 or more hexadecimal characters.
They are increasingly used to facilitate secure access over the internet protocols
to various Applications Programming Interfaces (APIs). Being plain text,
they are easily transmitted but they need to be stored securely,
that means strongly encrypted.

For similar trasport reasons, base64 encoding into printable characters is often used to encode binary data.



Security tokens and other types of files may sometimes be
all mixed up in one directory. This is not the best practice but
it may arise and it could involve much work to separate them.
`Ncrpt` copes with such mixed directories contents automagically.

## Installation

When using a typical Linux, the local `make` compilation may be skipped entirely. Then the binaries `symcrypt`, `hexcheck` and `hexify` that are included in this repository will be installed by default. They are compiled and tested automatically at github.com.

Otherwise a local installation should be repeated whenever the programs and scripts may have changed. Another good reason to perform a local compilation is if you suspect that the github binaries may have been compromised.

This software was tested under Linux. Installation from source needs `make` utility and a C compiler, either `clang` or `gcc`. Download or clone this directory, cd into it and then:

`sudo ./uninstall` will delete any previously installed programs and scripts and touch the local sources for recompilation.

`make CC=clang` or if clang compiler is not installed, just use the default compiler (under Linux it is usually gcc) with plain: **`make`**

`sudo ./install` copies all the executables for system-wide use into /usr/local/bin. Whether they were created by local compilation or just pulled from the repository. 

Alternatively, you can copy them manually to any of your own `bin` directories in your path. This does not require `sudo` privileges, e.g.:  
`cp symcrypt hexcheck hexify ncrpt dcrpt keygen crptest ~/bin`

The simplest complete installation method is to `touch *.c` and then invoke **`./crptest testing`** (from the root of the repository). This will compile the programs and, after asking for su priviledges, install them in `/usr/local/bin`.
As an added benefit, it will also run locally the same test as is done on github.

### Dependencies

Standard  **`base64`** tool which is normally pre-installed on Linux.

**`lzma`** is the default compression. It is normally pre-installed, otherwise install it with '`sudo apt-get install lzma`'.
  
**`zstd`** compression needs to be installed before you can explicitly select to use it with `-z` flag to `ncrpt`. To install: `sudo apt-get install zstd`.  
`dcrpt` issues a warning if `zstd` is not installed. This warning can be ignored if there are no `.zst` files to be decompressed.

There is not much difference in the compression rates: lzma
appears to be slightly better and zstd slightly faster.

## Usage

There are two command line interface bash scripts that automate the whole process:

`ncrpt [-h][-x][-b][-q][-v][-z] inputpath/dirname`

The options mean, respectively: -h help, -x test for hex files, -b test for base64 files, -q quiet, -v verbose, -z zstd compression. The tests for hex and base64 tests are now optional.  They only need to be invoked when the input directory contains such files. Should you forget to use them, everything will still work, only the output may take up more space than was strictly necessary.

Ncrpt creates two subdirectories in the current directory, each mirroring files in `inputpath/dirname`  that are to be encrypted. Thus
`./dirname_crp` will hold the encrypted files and  `./dirname_key` will hold unique keys individually generated for them. There is no recursive descent into subdirectories.

The summary at the end, such as the one shown in `test.log`, reports the sizes (in bytes) of input and output directories, the total compressed size as a percentage of the original size, and the total number of files encrypted. The compression to 50% will exactly compensate for the creation  of the encryption keys.

The quiet flag -q cancels the report.
The verbose flag -v adds the details of compressing each file. Setting both
flags, contradictory as that may seem, turns on the individual files reports and
turns off the final summary. The encryption itself is so unproblematic that it does not need any reports.

Summary: ncrpt (encrypt without vowels) executes the tasks of encoding recognition, compression selection, compression, key generation, key saving and encryption.

`dcrpt path/dirname_key path/dirname_crp`

is the inverse of `ncrpt`, i.e. its operations are carried out in exactly the reverse order.  It uses the keys in  `path/dirname_key` to decrypt their corresponding files, recognised by the same name in `path/dirname_crp`. So, never rename an encrypted file, unless you rename its corresponding key file as well! The two directories must always match.

Following decryption, the relevant decompression method(s) are applied to each file, so that the original files are exactly reconstructed.

The results are written into `./dirname_org` (under the current directory).

Summary: dcrpt, (decrypt without vowels) matches the keys, decrypts the binary files with them, selects the right decompression methods and  decompresses, thus reconstructing the contents of the original directory.

`crptest` optionally performs an automated overall test, checking that not a single byte was corrupted anywhere.

## Background Scripts and Programs (not needed by the user)

`hexcheck` (C executable) is invoked by `ncrpt`. It recognises hexadecimal (token) files and packs them to binary, which halves them in size.

`hexify` is invoked by `dcrpt` to unpack the binary files back to their original hexadecimal form.

`base64` recognises Base64 files, resulting in 25% size reduction in their case (before final compression). Base64 files should not contain any non base64 characters, such as newlines, as this test will then reject them.

`lzma` or `zstd` are the third party general compression methods used here for the final compression, as long as it will result in size reduction. This is not necessarily the case for small and/or binary files. Such incomressible files will be encrypted as they are.

When encryption is finally invoked, it is applied to the shortest possible form of each file, thus saving storage space for the data and for the keys. Decryption is the inverse of this process. See the scripts `ncrpt` and `dcrpt` for details. However, knowledge of the algorithms is not necessary for their effective use.

`symcrypt` (C executable) applies fast symmetric XOR encryption (or decryption) to any type of file of any length, while using practically no memory.

`keygen file > key` writes to stdout random binary data of the same length as the given file. Called by `ncrpt`.

`hexgen size`  writes to stdout `size` bytes of random hexadecimal data. No longer needed at all.

`b64gen size` writes to stdout `size` bytes of random base64 data. No longer needed at all.


## Testing

**`crptest`** inputpath/dirname 

Tests `ncrpt` and `dcrpt`. It first encrypts and then decrypts back again
all the files in the given input directory and compares the results against the original files.
It then cleans up all the created sub-directories.

Ideally, it should report all the reconstructed files as being identical to the originals.

An automated github action compiles the C programs and runs **`crptest`** over the `testing` directory included in the repository.
It tests all the main types of files: hexadecimal, base64, plain text and binary.
The 'test' badge at the top of this document lights up green
when all the tests were passed. Note that only the summary output `test.log` is saved in the repository after this automatic test, not the encrypted, decrypted or key directories.

## Releases Log

**23Nov2021** - Made multithreading more efficient: now converting files by background processes in order of their decreasing size. Should result in considerable speedups for large collections of large files.

**22Nov2021** - Improved `readme.md`. The key generation, encryption and decryption will now automatically run in sub shells on files over certain size, currently set to 10k. This will result in speedup on multi-core CPUs.

## References and Further Information

1. There is a blog [On Encryption and E-Democracy](https://oldmill.cz/2020-06-10-crypt.html) that describes in plain English the properties of XOR encryption and some interesting applications, primarily a model proposal for safe E-Democracy.
  
1. See also [FAQ.md](https://github.com/liborty/TokenCrypt/blob/master/FAQ.md) for frequently asked questions.
