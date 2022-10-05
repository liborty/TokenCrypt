# TokenCrypt [<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/liborty/tokencrypt/HEAD?logo=github">](https://github.com/liborty/tokencrypt) [![Actions Status](https://github.com/liborty/TokenCrypt/workflows/test/badge.svg)](https://github.com/liborty/TokenCrypt/actions)

**Efficiently compress and securely encrypt directories trees containing  hexadecimal security tokens, base64 data, text, binary and any other types of files. High security with ease of use.**

**Disclaimer:** use at your own risk! No warranties are given or implied. By downloading this software, you agree not to use it for unethical purposes.

## Security Advice

TokenCrypt uses theoretically unbreakable symmetric encryption but some practical precautions need to be adhered to.

The encryption keys and the encrypted files are packed and written into two separate output directories with user specified names. Optionally, and more securely, to two archive files.

Should the association between them be later lost or forgotten or one of them be lost entirely, then it will not be possible to reconstruct the original data! Therefore, it is recommended to record the chosen output names and to store them in a safe place, before ever deleting the original data. While using TokenCrypt as a local archiving and backup tool, the original data will normally be kept in place.

Also, before deleting any data, check that you have not forgotten to apply the recurse option `-r`, as in that case all the subdirectories will be missing from the archive.

The encrypted archive file is just meaningless random data and thus can be stored anywhere, even 'on the cloud'. The same applies to the keys archive file. The critical part is to prevent a potential eavesdropper from matching up those two files, as that is the only way to decrypt them. That is why they are not given related names automatically. Normally, the user should choose unrelated paths/names for them and keep them in separate places. Same as with public and private keys in assymetric encryption.

## Introduction

It is not an objective of TokenCrypt to replace `git` and to keep the complete histories of everything, as that carries significant costs in complexity and storage. The quest for being totally foolproof is subject to the law of diminishing returns, as foolishness generally knows no bounds. TokenCrypt is more akin to `borg`; an efficient archiver and backing up tool. The key difference, if excuse the pun, lies in TokenCrypt deploying more secure encryption and sometimes better compression as well. In its update mode, it only maintains the current version of the archive. Historical checkpoints are up to the user to create and keep, as and when required.

Scripts `ncrpt` and `expcrypt`read given input directory (tree) containing API tokens, base64 files and any other types of files. Subdirectories are processed recursively with -r option. `ncrpt` creates two output directories, `expcrypt` two archive files.

Some already compressed files afford only minor further compression, if any. To save time, they are recognised simply by their well known extensions:
`jpg,jpeg,mp4,zip,7z,lz,zst,gz,bz2` and their upper case versions.
All attempts at compressing these particular files are avoided. They are just encrypted as they are.

Given -xb options, `ncrpt` and `expcrypt` scripts will recognise hexadecimal files and base64 files and convert them to more compact binary data. They then select the best compression method individually for each file and finally securely encrypt them all.

Internet security tokens usually consist of 32, 64 or more hexadecimal characters. They are increasingly used to facilitate secure access over the internet protocols to various Applications Programming Interfaces (APIs). Even git uses hexadecimal 'slugs'. Being plain text, they are easily transmitted but they need to be stored securely, that means strongly encrypted.

Base64 encoding into printable characters is also generally used to encode binary data, for similar reasons.

Hexadecimal security tokens, base64 files and other types of files may sometimes be all mixed up in one common directory. This is not the best practice but it can arise. It could involve much work trying to separate them all.

`ncrpt` and `expcrypt` cope with such mixed contents directories by
applying data type recognition tests to each file individually. In this case, they do not rely on extension names, as that could lead to errors.

## Installation

This software was developed and tested under Linux. Installation from source needs `make` utility and a `C` compiler, e.g. `clang` or `gcc`. Download or clone this repository and cd into it. Then, for complete fresh installation: 

### `make install`

Will locally compile and install everything into `/usr/local/bin`. It may ask for su priviledges for the integrated installation step.

A good reason to perform local compilation is if it is suspected that the github binaries may have been compromised or when on a different machine architecture than `x86_64`. This step should be repeated whenever some programs and scripts have changed, such as after a fresh `git pull`. It will only recompile the Rust programs that have actually changed.

#### `make`

will only compile and install those Rust programs that have changed and none of the Bash scripts.

#### `sudo ./uninstall`

will delete previously installed programs and scripts. It can be used to force a completely fresh start.

#### `./install`

will install everything without the local compilation. Thus the ready made executables `symcrypt`, `hexcheck` and `hexify`, pulled from the repository, can be installed on `x86_64` architecture. They are compiled from Rust  sources and tested automatically at github.com (see the green badge at the top of this document).

Whether the executables were created by local compilation or just pulled from the repository, this script copies them all into `/usr/local/bin` for system-wide use.  Alternatively, they can be copied manually to any other `bin` directories and included in the search path. This does not require `sudo` privileges, e.g.:

#### `cp symcrypt hexcheck hexify ncrpt exprypt impcrypt keygen crptest expimptest ~/bin`

## Dependencies

Standard `du` (disk usage) tool which is normally part of Linux.

Standard  `base64` tool which is normally pre-installed on Linux.

`lzma` is the default compression. It is normally pre-installed. If not, install it with:  
`sudo apt-get install lzma`
  
`zstd` an alternative compression, selectable with `-z` option to `ncrpt`. It needs to be installed with:  
`sudo apt-get install zstd`

`tar` is used by `expcrypt` and `impcrypt` to pack the encrypted directory into a single archive file for more convenient and secure export/import.

`impcrypt` issues a warning if any of the compression utilities are not installed. It is probably best to install them all.
There is not much difference between lzma and zstd but they both have their fans. `lzma`
appears to have slightly better compression rate and `zstd` is slightly faster and more controllable. Of course, to unpack/decrypt archives on a new machine, it must have the same compression/decompression utility installed, too.

## Usage

There are three main command line interface (CLI) bash scripts that do most of the work and automate the whole process:

### `ncrpt [options] indir keydir outdir`

Long options introduced by `--` are also recognised.  
The options explained:

    -b --b64 test for base64 files, 
    -c --clean up the archive and the keys,
    -h --help,   
    -q --quiet, suppress the final report,
    -r --recurse, descend into subdirectories,
    -u --update an existing archive and keys,
    -v --verbose information on compressing each file
    -x --hex test for hexadecimal files, 
    -z --zstd compression to be used instead of lzma.     

When `-u` option is not given, then by default a new archive is created.  
Option `-c` only makes sense in combination with `-u`. It can do nothing for a brand new archive.

The tests for hexadecimal `-x` and base64 `-b` files should only be specified when the input directory likely contains such files. Though these tests are relatively fast. They usually fail after reading only a few bytes (of the wrong type of file). When these options are omitted by mistake, then everything will still work, only the default compression of these types of files will take up more space than was strictly necessary.

The last three arguments are mandatory: the input directory, the keys directory and the encrypted directory. Both output directories (`keydir` and `outdir`) will mirror `indir` in their structure and file names; `keydir` will hold the keys and `outdir` will hold the encrypted files.

The summary at the end, such as the one shown in `test.log`, reports the sizes (in bytes) of input and output directories and the total compressed size as a percentage of the original size. The compression to 50% will  compensate for the creation  of the encryption keys. Sometimes even better compression may be achieved.

Option `-q` (`--quiet`) cancels the final report.
Option `-v` (`--verbose`) adds details of compressing each file. Selecting  both options, contradictory as it may seem, turns on the individual files reports and turns off the final summary. The encryption itself is so unproblematic that it does not require any reports.

Once a directory has been compressed and encrypted, it is on subsequent occasions possible to update the keys and outdir directories (the archive) with option `-u`. This will any new files and recode updated ones. Added new files are marked with a: and more recent existing files are updated (marked with u:). When the recursive option -r is in use, the same actions will be applied to subdirectories. Capital letters A,U denote these two operations when applied to whole directories.

In order for the state of the new input directory and its archive to match again exactly one-to-one, an archive can be also cleaned up with option -c. Files no longer existing in indir will then be deleted (marked with d:) from the archive. With `-ruc` options, possibly whole directories can be deleted and marked with D:.

Caution should be exercised when using the -c option, as any files that had been inadvertently deleted from indir will then be removed from the archive as well. For added safety, option `-c` is deliberately made explicit and separate from `-u`. Thus using `-u` alone is the *cautious updating mode*, which never deletes anything from the archive. However, it will still overwrite existing files with their new, possibly erroneus, versions. Beware that this can cause a loss of previously useful content. Of course, this is true in general, whenever changing any files by any means, anywhere.

The most powerful mode of operation on a previously created archive (keydir outdir) is 'recurse-update-clean'.
Options can be run together:

```bash
ncrpt -ruc indir keydir outdir
```

This will recursively update and clean the archive so that it is as if freshly created from the current state of indir but it runs much faster. This is convenient for backing up purposes.

There is a vulnerability inherent in `ncrpt` creating directories. Specifically, specialist search engines sifting through the whole of the internet, possibly matching up pairs of (keydir outdir) directories by their same structures, file names and sizes, could in theory pair them up, even if they were uploaded to two unrelated places. Using `expcrypt` below is more secure.

It is recommended that `expcrypt` be used prior to exporting snapshots of the local indir to any unsecure locations, such as the internet.

The only reason to ever prefer `ncrpt` over `expcrypt` is when there is a lot of frequent updating of bulky directories to be done. This is because `ncrpt`, working with the directories, does not have to recreate the entire archive, is not tied to sequential processing and does fully utilise all the host computer cores for overall faster parallel execution.

Summary: `ncrpt` (encrypt with vowels left out) speedily executes the tasks of data type analysis, optimal compression selection, compression, key generation, key saving and encryption. Also recursive archiving and subsequent archive maintenance.

### `expcrypt [options] indir keyfile outfile`

Encrypt for export (expcrypt) works like `ncrpt` in the default 'create new archive' mode. Therefore,  the updating options `-u` and `-c` are no longer available here. All the other options are the same as in `ncrpt`. The most important difference is that instead of creating two recognisable output directories, `expcrypt` creates two output files that betray no similarities. They even differ in size.

All the keys for the whole archive are now packed into one sequential keyfile, which replaces previous keydir. No filenames are any longer being duplicated or extra file extensions appended. This is overall a cleaner solution.

The total of nine different combinations of compressions can still be used, as before, depending on the input file and its compressibility properties.

The compressed and encrypted files that used to go to outdir are now for convenience tarred together, also into a single file. If the individual filenames are confidential too, then this file should be encrypted once more.

There is a price to be paid in terms of the execution time. The entire indir tree structure now has to be traversed sequentially, thus reducing the opportunities for parallel execution (those are fully exploited by `ncrpt`).

### `impcrypt -[h][q][r][v] infile keyfile outdir`

Import and decrypt (impcrypt) unpacks, decrypts and decompresses directories created by `ncrpt` or archive files created by `expcrypt`. Both of its first two input arguments must be directories or both must be files. Its operations are carried out in exactly the reverse order.  It reads from indir (or from a tar archive file) the encrypted files. It also reads their associated keys from `keydir` (or from keys archive file). Individual files in input directories are paired up by their names, so never rename an encrypted file, unless you rename its corresponding key file as well. The root filenames in both directories must match. Within archive files produced by `expcrypt`, this issue does not arise.

Following decryption, the relevant decompression method(s) are applied to each file, so that the original files are exactly reconstructed in `outdir`. Please note that any strange types of system files or symbolic links might not be saved. Only genuine files that respond `true` to Bash `-f` test or genuine directories that respond `true` to `-d` test are saved.

The following can be done:

* `expcrypt -r indir keyfile outfile`
* upload `keyfile` and `outfile` to two different places on the internet 
* delete everything locally, intentionally or by accident:  
`rm -rf indir keyfile outfile`
* at any time and place later, download back `keyfile` and `outfile`, then
* `impcrypt -r outfile keyfile indir`

This should result in recovered original state of `indir`, securely saved in between.

Summary: `impcrypt` (import crypt) matches the keys, decrypts the indir files with them, selects the right decompression methods and  decompresses, thus reconstructing the exact contents of the original directory.

## Background Scripts and Programs (not needed by the user)

`hexcheck` (Rust executable)  
is invoked by `expcrypt -x`. It recognises hexadecimal (token) files and packs them to binary, which exactly halves them in size. Hexadecimal files should be an even number of bytes long and only contain (0-9,a-f) ascii characters. There are a few allowed  exceptions: upper case A-F are accepted but when converted back, they will always end up in lower case. Spaces and newlines just get deleted. This tolerant policy may result in some differences being reported between the original and the reconstructed files. Then it is best to replace the original file with its cleaned up, reconstructed version.

`hexify` (Rust executable)  
is invoked by `impcrypt` to unpack the binary files back to their original hexadecimal form. In other words, it carries out an inverse operation to `hexcheck` above.

`base64` (linux utility)  
is invoked by `expcrypt -b`. It recognises base64 files, resulting in 25% size reduction before the final general compression. Base64 files should not contain any non base64 characters, such as newlines, otherwise this strict test will reject them. This utility is also deployed in inverse mode by `impcrypt`.

`lzma` or `zstd` (linux utilities)  
are general compression methods used here for the final compression, as long as it result in size reduction. This is not necessarily the case for small and/or binary files. Any incomressible files are detected and encrypted as they are, even if their extensions are not on the 'do not compress' list. However, for extensions on the list, this test compression is avoided.

`symcrypt` (C executable)  
applies fast symmetric XOR encryption (or decryption). It XORs together two files of any kind but they must have the same length. The ordering of the two input files does not matter.

`keygen file > key`  
Bash script that writes to stdout random binary data of the same length as the given file. It is called by `expcrypt`.

`hexgen size`  
Bash script that writes to stdout `size` bytes of random hexadecimal data. It is only useful for generating test data.

`b64gen size`  
Bash script that writes to stdout `size` bytes of random base64 data. It is only useful for generating test data.

## Testing

An automated github action compiles the C programs and runs both `crptest`  and `expimptest` over the `testing` directory included in the repository.
It tests all the main types of files: hexadecimal, base64, plain text and binary. It also tests reursive descent into a subdirectory. The 'test' badge at the top of this document lights up green
when all the tests were passed. Note that only the summary output `test.log` is saved in the repository, everything else is tidied up (removed) after the tests.

Some character differences may arise for hexadecimal files because `hexcheck` converts both a-f and A-F to 10-15 and also it cleans up spurious spaces and newlines, instead of just rejecting such files. API keys should be separated into their own unique files. If the spaces/newlines turn out to be an unintended corruption, then the original file ought to be replaced by the cleaned up (reconstructed) version.

### `crptest testdir`

performs an automated test of `ncrpt`, creating directories, checking that not a single byte was corrupted anywhere while encrypting and decrypting back the contents of (any) `testdir`. The reported compression/decompression rates and sizes should also exactly match. Afterwards it cleans up all the created directories.

### `expimptest testdir`

is just like `crptest`, except it tests `expcrypt` and its output archive files instead of directories. In both cases, `impcrypt` is used for unpacking, decrypting and decompressing back. The reconstructed files should be reported as being identical to the originals.

## Exercise

Suppose you maintain some git repository, say `mygitrepo`. Go one level up:  `cd ..`, and run:  
`ncrpt -r mygitrepo mygitkeys gencrypted`  
Then, after some new trivial push from your repository, go back and run again:  
`ncrpt -ruc mygitrepo mygitkeys gencrypted`  
This will update only the file(s) that have changed. What may come as a surprise is the number of files added/changed by git in `.git`.

Note that TokenCrypt does not leave any such large hidden footprints on your filesystem.

## Releases Log

**5Sep22** - Release 1.0.5. Removed the long deprecated `dcrpt` script. Rewrote all three C programs in Rust, thus increasing security.

**19Dec21** - Release 1.0.3. Some code tidying and minor simplifications. No change in functionality. 

**18Dec21** - Release v1.0.2. Fixed orphaned jobs. Made tests more forgiving. They now tolerate changes in white space, which often happens with hex files.

**17Dec21** - Release v1.0.1. Fixed a bug in importing uncompressed files.

**16Dec21** - Breaking change: `ncrpt` now appends a single, one letter extension onto the keyfiles. `dcrpt` still reads the old key directories with their old compression encodings. It is now deprecated but left here for legacy reasons. It will eventually be withdrawn. Please use it as soon as possible to convert any existing old archives to the new format. `impcrypt` now automatically recognises the new format keys extensions, as well as the packed archive files. The tests have been amended to reflect this change.

**15Dec21** - `expcrypt` and `imcrypt` released for more secure and convenient exports and imports of archives snapshots.

**14Dec21** - Significant redesign of the new scripts.

**13Dec21** - First alpha release of `expcrypt` and `imcrypt` scripts. Not yet ready for general use. Keep using `ncrpt` and `dcrpt` for now.

**10Dec21** - Fixed misspelling bug in updatedir in `ncrpt` and modification time tests for directories, so the archive updates properly now. Improvements to readme manual. Prettier reports.

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

1. [Bash for Fun](https://www.lulu.com/shop/libor-spacek/bash-for-fun/ebook/product-nerj22.html?q=&page=1&pageSize=4)  
Some of the techniques used in TokenCrypt
are explained (amongst other things) in this book on Bash programming. 

1. There is a blog [On Encryption and E-Democracy](https://oldmill.cz/2020-06-10-crypt.html) that describes in plain English the properties of XOR encryption and some interesting applications, primarily a model proposal for safe E-Democracy.
  
1. Blog [Multithreading Automation](https://oldmill.cz/2021-11-24-joy-of-bashing2.html) describing the scheduling of the background tasks within this project.

1. See also [FAQ.md](https://github.com/liborty/TokenCrypt/blob/master/FAQ.md) for frequently asked questions.
