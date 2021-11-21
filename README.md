# TokenCrypt [<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/liborty/tokencrypt/HEAD?logo=github">](https://github.com/liborty/tokencrypt) [![Actions Status](https://github.com/liborty/TokenCrypt/workflows/test/badge.svg)](https://github.com/liborty/TokenCrypt/actions) 

**Efficiently compress and securely encrypt directories containing  hexadecimal security tokens, base64 data, text, binary or any other types of files. High security with ease of use.**

**Disclaimer:** use at your own risk! No warranties are given or implied. By downloading this software, you agree not to use it for unethical purposes.

**Warning:** the encryption keys are written to the subdirectory in the current directory. As is the encrypted directory. If you subsequently lose their connection or lose them entirely, it will not be possible to reconstruct your original data! Should you be encrypting many directories, take care to exactly mirror their directory structure with the generated key directories.

## Introduction

Internet security tokens usually consist of 32, 64 or more hexadecimal characters. 
They are increasingly used to facilitate secure access over the internet protocols
to various Applications Programming Interfaces (APIs). Being plain text,
they are easily transmitted but they need to be stored securely,
that means strongly encrypted.

For similar trasport reasons, base64 encoding into printable characters is often used to encode binary data.

`TokenCrypt` reads whole directories containing such tokens and/or any other types of files. It recognises hexadecimal files and base64 files and converts them to compact binary data. It then selects the best compression method for each file and finally encrypts them all with high security.

Security tokens and other types of files may sometimes be
all mixed up in one directory. This is not the best practice but
it may arise and it could involve much work to separate them.
`TokenCrypt` copes with such mixed contents automagically.

## Installation

Reminder: installation must be repeated locally every time that an updated repository is downloaded or pulled and the programs and scripts may have changed.

This software was tested under Linux. Installation from source needs a C compiler, either clang or gcc. Download or clone this directory, cd into it and then:

`make CC=clang` or if clang is not installed, just use the default compiler 
(under Linux it is usually gcc): **`make`**

When you are using a typical Linux, you can often skip the compilation step entirely. Then the automatically pre-compiled binaries `symcrypt` and `hexcheck`, that are included in this repository, will be installed by default.

`sudo ./install` copies all the executables for system-wide use into /usr/local/bin. To remove them again, use:  
`sudo ./uninstall`

Alternatively, you can copy them manually to any of your own `bin` 
directories in your path and this does not require `sudo` privileges, e.g.:

```bash
cp symcrypt hexcheck hexify ncrpt dcrpt keygen crptest ~/bin
```

The simplest installation is just to invoke **`./crptest testing`** from the root of the repository.
This will compile the programs and, after asking for su priviledges, install them in /usr/local/bin.
As an added benefit, it will also run locally the same test as on github.

### Dependencies

Standard  **`base64`** tool which is normally pre-installed on Linux.

The default compression used is lzma (.lz) but zstd (.zst) can be chosen with the -z flag.
There is not much difference in their compression rates but lzma
appears to be slightly better and zst slightly faster.

**`lzma`** compression is the default. It is normally pre-installed, otherwise install it with '`sudo apt-get install lzma`'.
  
**`zstd`** compression is only needed if you explicitly choose it by calling `ncrpt` with -z option. It will need installing first with: `sudo apt-get install zstd`, either before the above installation or at any time thereafter. `dcrpt` issues a warning if `zstd` is not installed. This warning can be ignored if there are no `.zst` files to be decompressed.

## Usage

There are two command line interface bash scripts that automate the whole process:

**`ncrpt** [-h][-x][-b][-q][-v][-z] inputpath/dirname`

The options mean, respectively: -h help, -x test for hex files, -b test for base64 files, -q quiet, -v verbose, -z zstd compression. The tests for hex and base64 tests are now optional.  They only need to be invoked when the input directory contains such files. Should you forget to use them, everything will still work, only the output may take up more space than was strictly necessary.

Ncrpt creates two subdirectories in the current directory, each mirroring files in `inputpath/dirname`  that are to be encrypted. Thus `./dirname_crp` will hold the encrypted files and  `./dirname_key` will hold unique keys individually generated for them. There is no recursive descent into subdirectories.

The summary at the end, such as the one shown in `test.log`, reports the sizes (in bytes) of input and output directories, the overall compression percentage rate and the total number of files encrypted.

The quiet flag cancels this report.
The verbose flag adds the details of compressing each file. Setting both
flags, contradictory as that may seem, turns on the individual files reports and
turns off the final summary. The encryption stage is so unproblematic that it does not need any reports.

Purpose: ncrpt (encrypt without vowels) executes the tasks of encoding recognition, compression selection, compression, key generation, key saving and encryption.

`dcrpt path/dirname_key path/dirname_crp`

is the inverse of `ncrpt`. It uses the keys in  `path/dirname_key` to decrypt
their corresponding files, recognised by the same name in `path/dirname_crp`.
All the decrypted results are written into `./dirname_org` (under the current directory).

Following decryption, the relevant decompression method(s) are applied to each file, so that the original files are exactly reconstructed.

Purpose: dcrpt, (decrypt without vowels) matches the keys, decrypts the binary files with them, selects the right decompression methods, decompresses, thus reconstructing the contents of the original directory.

`crptest` optionally performs an automated overall test, checking that not a single byte was corrupted anywhere.

## Background Scripts and Programs (not needed by the user)

`hexcheck` (C executable) is invoked by `ncrpt`. It recognises hexadecimal (token) files and packs them to binary, which halves them in size.

`hexify` is invoked by `dcrpt` to unpack the binary files back to their original hexadecimal form.


`base64` recognises Base64 files, resulting in 25% size reduction (before final compression),  in their case. Base64 files should not contain any non base64 characters, such as newlines, as this test then must reject  them.

`lzma` or `zstd` are the third party general compression methods used for the final compression, as long as it will result in size reduction. This is not necessarily the case for small and/or binary files. Such incomressible files will be encrypted as they are.

When encryption is finally invoked, it is applied to the shortest possible form of each file, thus saving storage space for the data and for the keys. Decryption is the inverse of this process. See the scripts `ncrpt` and `dcrpt` for details. Nevertheless, knowledge of the algorithms is not necessary for their effective use.

`symcrypt` (C executable) applies fast symmetric XOR encryption (or decryption) to any type of file of any length, while using practically no memory.


## Testing

**`crptest`** inputpath/dirname 

Tests `ncrpt` and `dcrpt`. It first encrypts and then decrypts back again
all the files in the given input directory and compares the results against the original files.
It cleans up after itself except for the keys directory `./dirname_key`,
which is left for reassurance and for information about how was each test file compressed.

Ideally there should be only a blank produced after "crptest found these differences:" However, not all .hex and .base64 files are perfect. Should they contain any spaces and newlines, these will be removed in the process and will cause differences to be reported.

An automated github action compiles the C programs and runs **`crptest`** over the `testing` directory included in the repository.
It tests all the main types of files: hexadecimal, base64, plain text and binary. 
The 'test' badge at the top of this document lights up green 
when all the tests were successful. Note that only the summary output `test.log` is saved in the repository after this automatic test, not the encrypted or key directories.

Should you want to set up your own tests, you  may find the following useful:

**`keygen`** file

Writes to stdout random binary data of the same length as the given file.

**`hexgen`** size

Writes to stdout `size` bytes of random hexadecimal data.

**`b64gen`** size

Writes to stdout `size` bytes of random base64 data.

## References and Further Information

1. There is a blog [On Encryption and E-Democracy](https://oldmill.cz/2020-06-10-crypt.html) that describes in plain English the properties of XOR encryption and some interesting applications, primarily a model proposal for safe E-Democracy.
  
1. See also [FAQ.md](https://github.com/liborty/TokenCrypt/blob/master/FAQ.md) for frequently asked questions.
