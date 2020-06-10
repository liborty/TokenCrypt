# TokenCrypt

**Encrypts and decrypts directories of security tokens and any other files. High security without onerous complications.**

## Version 1.1.5  [![Actions Status](https://github.com/liborty/TokenCrypt/workflows/test/badge.svg)](https://github.com/liborty/TokenCrypt/actions)

Use at your own risk! No warranties are given or implied. 
By downloading this software, you agree not to use it for unethical purposes.

## Outline

Internet security tokens usually consist of 32, 64 or more hexadecimal characters. 
They are increasingly used to facilitate secure access over the internet protocols
to various Applications Programming Interfaces (APIs). Being plain text,
they are easily transmitted but they need to be stored securely,
that means encrypted.   
  
For similar reasons, base64 encoding into printable characters is often used.

TokenCrypt takes whole directories of tokens
and/or any other files, recognises hexadecimal data files, recognises base64 data, chooses the best compression 
process individually for each file and finally encrypts them all with extremely high security.

Security tokens and other data may sometimes be 
all mixed up in common directories. This is not the best practice but 
it may well arise and could involve much work to separate them. 
`TokenCrypt` copes with such directories automagically.

The main workhorse at the heart of TokenCrypt is `symcrypt.c`, which applies fast symmetric
encryption or decryption to any type of file of any length, while using practically no memory.

Then there are two bash scripts that automate the whole process of encryption (`ncrpt`)
and decryption (`dcrpt`), subsuming the tasks of compression/decompression and keys generation as and when needed.
There is also an automated overall testing script `crptest`.

Entirely hexadecimal (token) files are recognised with `hexcheck` and converted to binary, which halves them in size. 

Base64 files are automatically recognised too, resulting in 25% size reduction in their case. Base64 files should not contain any newlines, as `base64` utility then rejects them. 

The overall gain after general compression may not be quite so much but it will still be significant, making the recognition of these files worthwhile.

Then either lzma or zstd general compression is applied, as long as it actually leads to a reduction of the file size. This is generally not  the case for small and/or binary files. 

Therefore, when  `ncrpt` is finally invoked to carry out the encryption, it is applied to the shortest form of each file.

Decryption is the inverse of this process. See the scripts `ncrpt` and `dcrpt` for details. Nevertheless, knowledge of the algorithms is not necessary for effective TokenCrypt use.

## Installation

Reminder: installation must be repeated locally every time that an updated repository is downloaded or pulled and the programs and scripts may have changed.

This software was tested under Linux. Installation from source needs a C compiler, either clang or gcc. Download or clone this directory, cd into it and then:

**`make CC=clang`**

or if clang is not installed, just use the default compiler 
(under Linux it is usually gcc): **`make`**

When you are using a typical Linux, you can often skip the compilation step entirely. Then the automatically pre-compiled binaries `symcrypt` and `hexcheck`, that are included in this repository, will be installed by default.

**`sudo ./install`**
 
Copies all the executables for system-wide use into /usr/local/bin.  
To remove them again, use: **`sudo ./uninstall`**

Alternatively, you can copy them manually to any of your own `bin` 
directories in your path and this does not require `sudo` privileges, e.g.:
```bash
cp symcrypt hexcheck hexify ncrpt dcrpt keygen crptest ~/bin
```
### Dependencies

Standard  **`base64`** tool which is normally pre-installed.  

**`lzma`** compression is now the default. It is normally pre-installed, otherwise install it with '`sudo apt-get install lzma`'.
  
**`zstd`** compression is only needed if you explicitly choose it by calling `ncrpt` with -z option. In which case it will need installing first with: '`sudo apt-get install zstd`', either before the above installation or at any time thereafter. `dcrpt` issues a warning when zstd is not installed. This warning can be ignored if there are no `.zst` files to be decompressed.

## Usage

**`ncrpt`** [-h][-x][-b][-q][-v][-z] inputpath/dirname

The options mean, respectively: -h help, -x test for hex files, -b test for base64 files, -q quiet, -v verbose, -z zstd compression. 

The tests for hex and base64 files are now optional, you only need to invoke them when you know that the directory being processed may contain some.

Encrypted output files go to `./dirname_crp` (under the current directory).
Unique new key is generated for each input file in `path/dirname` and
written to `./dirname_key` that exactly mirrors `./dirname_crp` (and `path/dirname`).

There is no recursive descent into subdirectories. Recursive version may be released later.

The default printout is just a summary report at the end, such as the one in `test.log`. It reports the sizes (in bytes) of input and output directories, the overall compression percentage rate and the total number of files encrypted.

The quiet flag cancels this report and that will normally mean no printouts at all.
The verbose flag adds the details of compressing each file. Setting both
flags, contradictory as that might seem, turns on the individual files reports and
turns off the final summary.
The encryption stage is so unproblematic that it does not need any reports.

The default compression used is lzma (.lz) but zstd (.zst) can be chosen with the -z flag.
There is not much difference in their compression rates but lzma
appears to be slightly better.

**`dcrpt`** path/dirname_key path/dirname_crp

is the inverse of `ncrpt`. It uses the keys in  `path/dirname_key` to decrypt 
their name corresponding files in `path/dirname_crp`.
All the decrypted results are written into `./dirname_org` (under the current directory).

Following decryption, it also automatically applies the correct decompression method(s) to each file.

There are also **`hexcheck`** and **`hexify`** programs which are used internally by  **`ncrpt`** and **`dcrpt`** respectively, to recognise/pack and unpack hexadecimal files.

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

## Further Information

See also [FAQ.md](https://github.com/liborty/TokenCrypt/blob/master/FAQ.md) for frequently asked questions.
