# TokenCrypt [<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/liborty/tokencrypt/HEAD?logo=github">](https://github.com/liborty/tokencrypt) [![Actions Status](https://github.com/liborty/TokenCrypt/workflows/test/badge.svg)](https://github.com/liborty/TokenCrypt/actions) 

**Efficiently compress and securely encrypt directories trees containing  hexadecimal security tokens, base64 data, text, binary and any other types of files. High security with ease of use.**

**Disclaimer:** use at your own risk! No warranties are given or implied. By downloading this software, you agree not to use it for unethical purposes.

**Warning:** the encryption keys and the encrypted files are written to two separate directories. Should you subsequently lose track of the association between them or lose the keys entirely, it will not be possible to reconstruct your original data! When encrypting many different directories, take care to record the mappings.

## Introduction

`ncrpt` reads given input directory containing API tokens, base64 files and
any other types of files. It recognises hexadecimal files and base64 files and converts them to compact binary data.
It selects the best compression method individually for each file and finally encrypts them all with high security.
Subdirectories are processed recursively with -r option.

Internet security tokens usually consist of 32, 64 or more hexadecimal characters.
They are increasingly used to facilitate secure access over the internet protocols
to various Applications Programming Interfaces (APIs). Being plain text,
they are easily transmitted but they need to be stored securely,
that means strongly encrypted.

For similar trasport reasons, base64 encoding into printable characters is often used to encode binary data.

Security tokens and other types of files may sometimes be
all mixed up in one directory. This is not the best practice but
it can arise and it could involve much work to separate them.
`ncrpt` copes with such mixed directories contents automatically.

## Installation

When using a typical Linux, the local `make` compilation may be skipped entirely. Then the binaries `symcrypt`, `hexcheck` and `hexify` that are included in this repository will be installed by default. They are compiled from `C` sources and tested automatically at github.com.

Otherwise a local installation should be repeated whenever the programs and scripts may have changed. Another good reason to perform a local compilation is if you suspect that the github binaries may have been compromised.

This software was tested under Linux. Installation from source needs `make` utility and a `C` compiler, either `clang` or `gcc`. Download or clone this directory, cd into it and then:

`sudo ./uninstall` will delete any previously installed programs and scripts and touch the local sources for recompilation.

`make CC=clang` or if clang compiler is not installed, just use the default compiler (under Linux it is usually gcc) with plain: **`make`**

`sudo ./install` copies all the executables for system-wide use into /usr/local/bin. Whether they were created by local compilation or just pulled from the repository.

Alternatively, you can copy them manually to any of your own `bin` directories in your path. This does not require `sudo` privileges, e.g.:  
`cp symcrypt hexcheck hexify ncrpt dcrpt keygen crptest ~/bin`

The simplest complete installation method is to `touch *.c` and then, at the root of the repository, invoke:  
`./crptest testing`  
This will compile the programs and, after asking for su priviledges, install them in `/usr/local/bin`.
As an added benefit, it will also run locally the same test as is done on github.

## Dependencies

Standard  **`base64`** tool which is normally pre-installed on Linux.

**`lzma`** is the default compression. It is normally pre-installed, otherwise install it with:  
`sudo apt-get install lzma`
  
**`zstd`** compression needs to be installed before you can explicitly select to use it with `-z` flag to `ncrpt`. To install: `sudo apt-get install zstd`.

`dcrpt` issues a warning if any of the utilities are not installed. It is probably best to install them all.
There is not much difference between lzma and zstd but they both have their fans. lzma
appears to have slightly better compression and zstd is slightly faster and more controllable.

## Usage

There are two command line interface bash scripts that do most of the work and automate the whole process:

`ncrpt [-h][-x][-b][-q][-r][-v][-z] indir keydir outdir`

The options mean, respectively: -h help, -x test for hexadecimal files, -b test for base64 files, -q quiet, -r recurse, -v verbose, -z zstd compression. Their -- long versions are also accepted.

The tests for hexadecimal and base64 files only need to be invoked when the input directory contains such files. Should you forget to use them, everything will still work, only the output may take up more space than was strictly necessary.

The last three arguments are mandatory: the input directory, the (output) keys directory and the output encrypted diretory. Both output directories will mirror the directories and filenames in `indir`. Thus keydir will hold the keys and outdir will hold the encrypted files.

The summary at the end, such as the one shown in `test.log`, reports the sizes (in bytes) of input and output directories and the total compressed size as a percentage of the original size. The compression of 50% will  compensate for the creation  of the encryption keys. Sometimes it will be even better.

The quiet flag -q cancels the report.
The verbose flag -v adds the details of compressing each file. Setting both
flags, contradictory as that may seem, turns on the individual files reports and
turns off the final summary. The encryption itself is so unproblematic that it does not need any reports.

Summary: `ncrpt` (encrypt without vowels) executes the tasks of data type analysis, compression selection, compression, key generation, key saving and encryption.

`dcrpt [-h][-q][-r][-v] indir keydir outdir`

is the exact inverse of `ncrpt` and its operations are carried out in  the reverse order.  It reads the encrypted files from indir and their keys from keydir. They are paired up by their same filenames. So, never rename an encrypted file, unless you rename its corresponding key file as well! The two directories must always match.

Following decryption, the relevant decompression method(s) are applied to each file, so that the original files are exactly reconstructed. The compression methods are recorded for each file in the names of the extension(s) of its keyfile.

The results are written into outdir.

Summary: dcrpt, (decrypt without vowels) matches the keys, decrypts the binary files with them, selects the right decompression methods and  decompresses, thus automatically reconstructing the exact contents of the original directory.

`crptest` optionally performs an automated overall test, checking that not a single byte was corrupted anywhere.

## Security Consideration

The generated keys on their own are just meaningless random data and thus can be stored anywhere without compromising the security. The same applies to the encrypted files on their own. The security critical part is to prevent a potential eavesdropper from matching them up with each other.

## Background Scripts and Programs (not needed by the user)

`hexcheck` (C executable) is invoked by `ncrpt`. It recognises hexadecimal (token) files and packs them to binary, which halves them in size. Hexadecimal files should only contain (0-9,A-F) ascii characters. There are some exceptions: lower case a-f are accepted but converted to A-F. Spaces and newlines just get deleted. This tolerant policy may result in some differences between the original and the reconstructed files. Then it is best to replace the original file with the cleaned up reconstructed one.

`hexify` is invoked by `dcrpt` to unpack the binary files back to their original hexadecimal form.

`base64` recognises Base64 files, resulting in 25% size reduction in their case (before final compression). Base64 files should not contain any non base64 characters, such as newlines, as this test will then reject them.

`lzma` or `zstd` are the third party general compression methods used here for the final compression, as long as it will result in size reduction. This is not necessarily the case for small and/or binary files. Such incomressible files will be encrypted as they are.

`symcrypt` (C executable) applies fast symmetric XOR encryption (or decryption) to any type of file of any length, while using practically no memory.

`keygen file > key` writes to stdout random binary data of the same length as the given file. Called by `ncrpt`.

`hexgen size`  writes to stdout `size` bytes of random hexadecimal data. No longer needed at all.

`b64gen size` writes to stdout `size` bytes of random base64 data. No longer needed at all.

## Testing

`crptest indir`

Tests `ncrpt` and `dcrpt`. It first encrypts and then decrypts 
all the files in the given input directory and compares the results against the original files. It then cleans up all the created directories.

`crptest` should report all the reconstructed files as being identical to the originals.
Some differences may be reported for hexadecimal files because `hexcheck` converts a-f to A-F and cleans up spurious spaces and newlines, instead of just rejecting such almost hexadecimal files. API keys should be separated into their own  unique files. If the spaces/newlines turn out to be an unintended corruption, then the original file ought to be replaced by the cleaned up (reconstructed) version.

An automated github action compiles the C programs and runs **`crptest`** over the `testing` directory included in the repository.
It tests all the main types of files: hexadecimal, base64, plain text and binary. It also tests reursive descent into a subdirectory.
The 'test' badge at the top of this document lights up green
when all the tests were passed. Note that only the summary output `test.log` is saved in the repository after this automatic test, not the encrypted, decrypted or key directories.

## Releases Log

**26Nov21** - Added `-r --recurse` option, so that we now have a proper archiver. Adopted `getopts` options processing. Generally fortified the code.

**24Nov21** - Shortened .base64 extensions to .b64. Rename old encrypted files accordingly. Some cosmetic improvements.

**23Nov21** - Made multithreading more efficient: now converting files by background processes in order of their decreasing size. Should result in considerable speedups for large collections of large files.

**22Nov21** - Improved `readme.md`. The key generation, encryption and decryption will now automatically run in sub shells on files over certain size, currently set to 10k. This will result in speedup on multi-core CPUs.

## References and Further Information

1. There is a blog [On Encryption and E-Democracy](https://oldmill.cz/2020-06-10-crypt.html) that describes in plain English the properties of XOR encryption and some interesting applications, primarily a model proposal for safe E-Democracy.
  
1. See also [FAQ.md](https://github.com/liborty/TokenCrypt/blob/master/FAQ.md) for frequently asked questions.
