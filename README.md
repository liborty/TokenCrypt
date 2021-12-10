# TokenCrypt [<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/liborty/tokencrypt/HEAD?logo=github">](https://github.com/liborty/tokencrypt) [![Actions Status](https://github.com/liborty/TokenCrypt/workflows/test/badge.svg)](https://github.com/liborty/TokenCrypt/actions) 

**Efficiently compress and securely encrypt directories trees containing  hexadecimal security tokens, base64 data, text, binary and any other types of files. High security with ease of use.**

**Disclaimer:** use at your own risk! No warranties are given or implied. By downloading this software, you agree not to use it for unethical purposes.

## Security Advice

The encryption keys and the encrypted files are written into two separate directories. If the association between them is lost/forgotten or the keys are lost entirely, then it is impossible to reconstruct the  original data! Therefore, when encrypting many different directories trees, it is recommended to record their key mappings and to store them in a safe place, before ever deleting the original data.

When using TokenCrypt as a back up and archiving tool, the original data will normally be kept.

The encrypted files (written to outdir) are just meaningless random data and thus can be stored anywhere (even 'on the cloud'), without compromising the security. The same applies to the keys (in keydir), although these reveal the type of compression that was used. The critical part is to prevent a potential eavesdropper from matching up those two directories, as that is the only way to decrypt them.

## Introduction

It is not an objective of TokenCrypt to replace `git` and to keep the complete histories of everything, with its significant costs in complexity and storage. The quest for being totally foolproof is subject to the law of diminishing returns. We do not know for sure if the universe is infinite but we do know that human foolishness certainly is. Therefore, TokenCrypt is more akin to `borg`; an efficient archiver and backing up tool. The key difference, if excuse the pun, lies in TokenCrypt deploying far more secure encryption and sometimes better compression as well.

`ncrpt` reads given input directory (tree) containing API tokens, base64 files and
any other types of files. Subdirectories are processed recursively with -r option.
This script recognises hexadecimal files and base64 files and converts them to compact binary data.
It then selects the best compression method individually for each file and finally encrypts them all with total security.

Some well known compressed formats afford only minor further compression, if any.
To save time, they are recognised here simply by their well known extensions:
`jpg,mp4,zip,7z,lz,zst,gz,bz2` and their upper case versions.
All attempts at compressing these particular files are avoided.

Internet security tokens usually consist of 32, 64 or more hexadecimal characters.
They are increasingly used to facilitate secure access over the internet protocols
to various Applications Programming Interfaces (APIs). Being plain text,
they are easily transmitted but they need to be stored securely,
that means strongly encrypted.

For similar transport reasons, base64 encoding into printable characters is often used to encode binary data.

Security tokens and other types of files may sometimes be
all mixed up in one common directory. This is not the best practice but
it can arise and it could involve much work trying to separate them.
`ncrpt -b -x` copes with such mixed directories contents by
using automatic data type recognition for each file individually.

## Installation

This software was developed and tested under Linux. Installation from source needs `make` utility and a `C` compiler, e.g. `clang` or `gcc`. Download or clone this repository and cd into it. Then, for complete fresh installation:  
### `./crptest testing`  
This will compile all three `C` programs and, after asking for su priviledges, install them and the bash scripts in `/usr/local/bin`. As an added benefit, it will also run locally the same test as is done on github.

### Manual alternatives:

#### `sudo ./uninstall`  
will delete previously installed programs and scripts. Use to force a completely fresh start. To be followed by:

#### `./install`  
This installs everything without the local compilation. Thus the ready made executables `symcrypt`, `hexcheck` and `hexify`, pulled from the repository, can be installed on `x86_64` architecture. They are compiled from `C` sources and tested automatically at github.com (see the green badge at the top of this document).

Whether the executables were created by local compilation or just pulled from the repository, this script copies them all into `/usr/local/bin` for system-wide use.  Alternatively, they can be copied manually to any other `bin` directories and included in the search path. This does not require `sudo` privileges, e.g.:  
`cp symcrypt hexcheck hexify ncrpt dcrpt keygen crptest ~/bin`

#### `make install`  
Will compile and install everything into `/usr/local/bin`. It may ask for su priviledges for the integrated installation step. Appending `CC=clang` will deploy the clang compiler (if installed). 

A good reason to perform local compilation is if you suspect that the github binaries may have been compromised or you have some diferent machine architecture. This step should be repeated whenever some programs and scripts have changed, such as after a fresh `git pull`. It will only recompile the C programs that have actually changed.

#### `make`
This will only compile and install those C programs that have changed and none of the Bash scripts.

## Dependencies

Standard `du` (disk usage) tool which is normally part of Linux.

Standard  `base64` tool which is normally pre-installed on Linux.

`lzma` is the default compression. It is normally pre-installed. If not, install it with:  
`sudo apt-get install lzma`
  
`zstd` an alternative compression, selected with `-z` option to `ncrpt`. It needs to be installed with:  
`sudo apt-get install zstd`

`dcrpt` issues a warning if any of the utilities are not installed. It is probably best to install them all.
There is not much difference between lzma and zstd but they both have their fans. `lzma`
appears to have slightly better compression rate and `zstd` is slightly faster and more controllable. Of course, to unpack/decrypt archives on a new machine, it must have the same compression/decompression utility installed, too.

## Usage

There are two command line interface (CLI) bash scripts that do most of the work and automate the whole process:

### `ncrpt [-b][-c][-h][-q][-r][-u][-v][-x][-z] indir keydir outdir`

The long options introduced by `--` are also recognised.
The options explained:

    -b --b64 test for base64 files, 
    -c --clean up the archive and the keys,
    -h --help,   
    -q --quiet suppress the final report,
    -r --recurse descend into subdirectories,
    -u --update an existing archive and keys,
    -v --verbose information on compressing each file
    -x --hex test for hexadecimal files, 
    -z --zstd compression to be used instead of lzma.     

The tests for hexadecimal and base64 files only need to be selected when the input directory likely contains such files. They are quick, as they usually fail after reading only a few bytes (of the wrong type of file). Should you forget to select them, everything will still work, only the default compression may take up more space than was strictly necessary.

The last three arguments are mandatory: the input directory, the keys directory and the encrypted directory. Both output directories (`keydir` and `outdir`) will mirror `indir` in their structure and file names; `keydir` will hold the keys and `outdir` will hold the encrypted files.

The summary at the end, such as the one shown in `test.log`, reports the sizes (in bytes) of input and output directories and the total compressed size as a percentage of the original size. The compression to 50% will  compensate for the creation  of the encryption keys. Sometimes even better compression may be achieved.

Option `-q` (`--quiet`) cancels the final report.
Option `-v` (`--verbose`) adds details of compressing each file. Setting both flags, contradictory as it may seem, turns on the individual files reports and turns off the final summary. The encryption itself is so unproblematic that it does not require any reports.

Once a directory has been compressed and encrypted, it is on subsequent occasions possible to update the keys and outdir directories (the archive) with the option -u. This will add or recode just the new and updated files. New files are added (marked with a:) and more recent existing files are updated (u:). When the recursive option -r is in use, the same will be applied to subdirectories. Capital letters A,U denote these two operations when applied to whole directories.

In order for the state of the new indir and its archive to match again exactly and one-to-one, an archive can be also cleaned up with option -c. Files no longer existing in indir will then be deleted (d:) from the archive. With `-r -c` options, possibly whole directories can be deleted, reported with (D:).

Caution should be exercised when using the -c option, as any files that had been inadvertently deleted from indir will then be removed from the archive as well. For added safety, option `-c` is deliberately explicit and separate from `-u`. Thus using `-u` alone is the *cautious updating mode*, which never deletes from the archives. However, it will still overwrite with new erroneous versions of existing files.

The most powerful use (on an existing archive: keydir outdir) is:

```bash
ncrpt -r -u -c indir keydir outdir
```

This will recursively update and clean the archive so that it is as if freshly created from the current state of indir. This is convenient for backing up purposes.

Summary: `ncrpt` (encrypt with vowels left out) executes the tasks of data type analysis, optimal compression selection, compression, key generation, key saving and encryption. Also recursive archiving and subsequent archive maintenance.

### `dcrpt [-h][-q][-r][-v] indir keydir outdir`

is the conceptual inverse of `ncrpt`, although it is simpler. Its operations are carried out in  the reverse order.  It reads from indir the encrypted files previously created by `ncrpt` and it also reads their associated keys from `keydir`. They are paired up by their filenames. So, never rename an encrypted file, unless you rename its corresponding key file as well! The two directories must always match.

Following decryption, the relevant decompression method(s) are applied to each file, so that the original files are exactly reconstructed in `outdir`. The compression method(s) were recorded for each file in the names of the extension(s) of its keyfile.

Summary: `dcrpt` (decrypt with vowels left out) matches the keys, decrypts the binary indir files with them, selects the right decompression methods and  decompresses, thus reconstructing the exact contents of the original directory.

### `crptest testdir`

optionally performs an automated overall test, checking that not a single byte was corrupted anywhere while encrypting and decrypting back the contents of (any) `testdir`.

## Background Scripts and Programs (not needed by the user)

`hexcheck` (C executable) is invoked by `ncrpt`. It recognises hexadecimal (token) files and packs them to binary, which exactly halves them in size. Hexadecimal files should be an even number of bytes long and only contain (0-9,a-f) ascii characters. There are some allowed  exceptions: upper case A-F are accepted but when converted back will always end up in lower case. Spaces and newlines just get deleted. This forgiving policy may result in some differences between the original and the reconstructed files. Then it is best to replace the original file with its cleaned up, reconstructed version.

`hexify` (C executable) is invoked by `dcrpt` to unpack the binary files back to their original hexadecimal form.

`base64` (linux utility) recognises base64 files, resulting in 25% size reduction in their case (before final compression). Base64 files should not contain any non base64 characters, such as newlines, as this test will reject them.

`lzma` or `zstd` (linux utilities) are general compression methods used here for the final compression, as long as it will result in size reduction. This is not necessarily the case for small and/or binary files, such as .jpg images. Such mostly incomressible files will be encrypted as they are.

`symcrypt` (C executable) applies fast symmetric XOR encryption (or decryption). It XORs together two files of any kind but they must have the same length. The ordering of the two input files does not even matter.

`keygen file > key` writes to stdout random binary data of the same length as the given file. Called by `ncrpt`.

`hexgen size`  writes to stdout `size` bytes of random hexadecimal data. Only useful for generating test data.

`b64gen size` writes to stdout `size` bytes of random base64 data. Only useful for generating test data.

## Testing

`crptest indir`

Tests `ncrpt` and `dcrpt`. It first encrypts and then decrypts
all the files in the given input directory and compares the results against the original files. Finally, it cleans up all the created directories.

The reported compression/decompression rates and sizes should exactly match.

The reconstructed files should be reported as being identical to the originals.
Some character differences may arise for hexadecimal files because `hexcheck` converts both a-f and A-F to 10-15 and also it cleans up spurious spaces and newlines, instead of just rejecting files. API keys should be separated into their own  unique files. If the spaces/newlines turn out to be an unintended corruption, then the original file ought to be replaced by the cleaned up (reconstructed) version (see FAQ.md).

An automated github action compiles the C programs and runs **`crptest`** over the `testing` directory included in the repository.
It tests all the main types of files: hexadecimal, base64, plain text and binary. It also tests reursive descent into a subdirectory.
The 'test' badge at the top of this document lights up green
when all the tests were passed. Note that only the summary output `test.log` is saved in the repository, not the encrypted, decrypted or key directories.

## Releases Log

**10Dec21** - Minor improvements to readme manual. Fixed misspelling bug in updatedir in `ncrpt` and modification time tests for directories, so the archive updates properly now.

**9Dec21** - Added option -c to `ncrpt` for cleaning up archive and keys by removing directories and files which had been deleted from the input directory. Added a report how many cores of the user's machine are being used.

**8Dec21** - Added option -u to `ncrpt` for updating compressed encrypted archives.

**29Nov21** - Added recognition of some well known compressed formats by their extensions, to avoid compressing them again (speed up). Fixed silent cleaning up of temp files. Added some minor clarifications to this file (README.md). Added timing to `crptest`. This should be a stable version now. Enjoy!

**28Nov21** - Added buffering to `symcrypt`. It now for added security fails when the lengths of its two input files do not match. Also improved the `makefile`.

**27Nov21** - Added buffering to further enhance performance. Also, `hexgen` and `hexcheck` are now generating and converting all hex characters A-F consistently to a-f. Updated FAQ.md.

**26Nov21** - Added `-r --recurse` option, so that we now have a proper archiver. Adopted `getopts` options processing. Generally fortified the code.

**24Nov21** - Shortened .base64 extensions to .b64. Rename old encrypted files accordingly. Some cosmetic improvements.

**23Nov21** - Made multithreading more efficient: now converting files by background processes in order of their decreasing size. Should result in considerable speedups for large collections of large files.

**22Nov21** - Improved `readme.md`. The key generation, encryption and decryption will now automatically run in sub shells on files over certain size, currently set to 10k. This will result in speedup on multi-core CPUs.

## References and Further Information

1. There is a blog [On Encryption and E-Democracy](https://oldmill.cz/2020-06-10-crypt.html) that describes in plain English the properties of XOR encryption and some interesting applications, primarily a model proposal for safe E-Democracy.
  
1. Blog [Multithreading Automation](https://oldmill.cz/2021-11-24-joy-of-bashing2.html) describing the scheduling of the background tasks within this project.

1. See also [FAQ.md](https://github.com/liborty/TokenCrypt/blob/master/FAQ.md) for frequently asked questions.
