# TokenCrypt [<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/liborty/tokencrypt/HEAD?logo=github">](https://github.com/liborty/tokencrypt) [![Actions Status](https://github.com/liborty/TokenCrypt/workflows/test/badge.svg)](https://github.com/liborty/TokenCrypt/actions)

**Efficiently compress and securely encrypt directories trees containing  hexadecimal security tokens, base64 data, text, binary and any other types of files. High security combined with ease of use.**

**Disclaimer:** use at your own risk! No warranties are given or implied. By downloading this software, you agree not to use it for unethical purposes.

## Security Advice and Known Vulnerabilities

`TokenCrypt` uses theoretically unbreakable symmetric encryption. However, the following practical precautions need to be followed:

Runs only on systems where `/dev/urandom` returns truly random bytes. Better not use it for large archives immediately after reboot, as the OS may not have yet gathered enough physical randomness from user interactions. Nor on 'headless' servers, where there are no mouse and keyboard interactions.

The encryption keys and the compressed and encrypted files are written by `ncrpt` into two separate output directories with user specified names. They are restored by the use of `dcrpt`. This is suitable for local use only.

For exports, use `pack` (and `unpack`), which archive (and restore) the directory tree to/from three flat archive files. This option is more compact and significantly more secure in transit and storage.

Should the implicit association between these three files be lost or forgotten, then it will not be possible to reconstruct the original data! Therefore, it is recommended to record the chosen output names and to store them in a safe place, before ever deleting the original data.

The encrypted archive files are just meaningless 'random' data and thus can be stored anywhere, even 'on the cloud'. The critical part is to prevent a potential eavesdropper from matching up these three files, as that is the only way to decrypt them. That is why they are not given related names automatically. Normally, the user should choose unrelated paths/names for them and keep them well separated. As with private keys in asymmetric encryption.

Previous versions had a vulnerability: it was in theory possible to find a match somewhere on the internet between the `keyfile` and the `outfile`, based on the fact that they were both of the same size. As of `v1.1.0`, it is much harder to guess the right combination of three different files, all three of different sizes, with different names and stored in different places (it goes without saying that their names and places should be unrelated). 

## Introduction

It is not an objective of TokenCrypt to replace `git` and to keep the complete histories of everything, as that carries significant costs in complexity and storage. The quest for being totally foolproof is subject to the law of diminishing returns, as human foolishness generally knows no bounds. TokenCrypt is more akin to `borg`: an efficient archiver and backing up tool. The key difference, please excuse the pun, lies in TokenCrypt deploying more secure encryption and somewhat better compression as well. In its update mode, it only maintains the current version of the archive. Historical checkpoints are up to the user to create and keep, as and when required.

Scripts `pack` and `ncrpt` read given input directory tree, containing any types of files: `ncrpt` creates two output directories, `pack` creates three flat archive files.

Some already compressed files afford only minor further compression, if any. To save time, they are recognised simply by their well known extensions: `jpg, jpeg, mp4, zip, 7z, lz, zst, gz, tgz, bz2`, and their upper case versions. Files with these extensions are encrypted uncompressed.

Given `-xb` options, `ncrpt` and `pack` scripts will recognise hexadecimal files (*) and base64 files respectively and convert them to more compact binary data. For hexadecimal files, this results in 50% compression. They then select the best final compression method for each individual file and finally securely encrypt them all.

(*) Internet security tokens usually consist of 32, 64 or more hexadecimal characters. They are increasingly used to facilitate secure access over the internet protocols to various Applications Programming Interfaces (APIs). Even git uses hexadecimal 'slugs'. Being plain text, they are easily transmitted but they need to be stored securely, that means strongly encrypted. Base64 encoding into printable characters is also generally used to encode binary data, for similar reasons.

Hexadecimal security tokens, base64 files and all other types of files may sometimes be mixed up in one common directory. This is not the best practice but it does arise. It could involve much work trying to separate them all. `ncrpt` and `pack` cope with such mixed contents directories by applying data type recognition tests to each file individually. In this case, they do not rely on extension names, as that could lead to errors.

## Installation

This software was developed and tested under Linux and the latest binaries are available in the repository as a `release`. Installation from source needs Rust installed. Download or clone this repository (to get the latest version of the bash scripts) and `cd` into it. Then, for complete fresh installation:

### `sudo ./install`

This will locally compile four Rust  programs `xorfork`, `symcrypt`, `hexify` and `hexcheck`. Everything that may have changed is installed into `/usr/local/bin`. The bash scripts are also installed, though, of course, they do not need any compilation. For those that do not want to install Rust, there are x86-64 binaries available in the 'releases' in the repository.

The Rust sources are also automatically compiled and tested at `github.com` - see the green badge at the top of this document. Whether the executables were created by local compilation or just pulled from the repository, this script copies them all from `.bin/` into `/usr/local/bin` for system-wide use.  Alternatively, they can be copied manually to any other `bin` directories that are included in the search path. That does not require `sudo` privileges.

A good reason to perform local compilation is if it is suspected that the `github` binaries may have been compromised or when using a different machine architecture. This step should be repeated whenever some programs and scripts have changed, such as after performing a new  `git pull`.

### `sudo ./uninstall`

will delete previously installed programs and scripts. It can be used to enforce a completely fresh start.

## Dependencies

`/dev/urandom` - standard source of truly random data.

`du` (disk usage) - standard tool which is part of Linux.

`base64` tool which is also normally pre-installed on Linux.

`lzma` is the default compression utility. It is normally pre-installed. If not, install it with: `sudo apt-get install lzma`
  
`zstd` is an alternative compression, selectable with `-z` option to `ncrpt` and `pack`. It usually needs to be installed with: `sudo apt-get install zstd`

`dcrpt` and `unpack` issue warnings if any of the necessary utilities are not installed. It is probably best to install them all. There is not much difference between `lzma` and `zstd` compressions but they each have their own fans. `lzma` appears to have slightly better compression rate and `zstd` is slightly faster and more controllable. Of course, to unpack/decrypt archives on a new machine, it must have the same compression/decompression utility installed, too.

## Usage

There are four command line interface (CLI) bash scripts that do all the work and automate the whole process:

### `ncrpt [options] indir keydir outdir`

This script performs data type analysis, optimal compression selection, compression, key generation, key saving and encryption. Also recursive archiving and subsequent archive maintenance.

Long options introduced by `--` are also recognised.  
The options explained:

    -b --b64 test for base64 files, 
    -c --clean up the archive and the keys,
    -h --help,
    -i --ignore, do not descend into subdirectories   
    -q --quiet, suppress the final report,
    -u --update an existing archive and keys,
    -v --verbose information on compressing each file
    -x --hex test for hexadecimal files, 
    -z --zstd compression to be used instead of lzma.     

When `-u` option is not given, then by default a new archive is created.  
Option `-c` only makes sense conjoined in `-uc` because it can do nothing for a brand new archive.

The tests for hexadecimal `-x` and base64 `-b` files should only be specified when the input directory likely contains such files. However, these tests are fast. They usually fail after reading only a few bytes (of the wrong type of file). When these options are omitted by mistake, then everything will still work, only the default compression of these types of files will take up more space than was strictly necessary.

The three arguments are mandatory: the input directory, the keys directory and the encrypted directory. Both output directories (`keydir` and `outdir`) will mirror `indir` in their structure and file names; `keydir` will hold the keys and `outdir` will hold the encrypted files.

The summary at the end, such as the one shown in `test.log`, reports the sizes (in bytes) of input and output directories and the total compressed size as a percentage of the original size. Compression to 50% would  compensate for the creation of the encryption keys. Sometimes even better compression may be achieved.

Option `-q` (`--quiet`) cancels the final report.
Option `-v` (`--verbose`) adds details of compressing each file. Selecting  both options `-vq`, contradictory as it may seem, turns on the individual file reports and turns off the final summary. The encryption itself is so unproblematic that it does not require any reports.

Once a directory has been compressed and encrypted, it is on subsequent occasions possible to update the keys and outdir directories (the archives) with option `-u`. This will add any new files and recode updated ones. Added new files are marked with a: and more recent existing files are updated (marked with u:). The same actions will be applied to all subdirectories, unless option `-i` has been deployed. Capital letters A:,U: denote these two operations when applied to whole subdirectories.

An archive can additionally be cleaned up with `-uc`, to ensure that a new state of the input directory and its archives match exactly one-to-one. That is, files no longer existing in indir will be deleted from the archive (and marked with d:). Unless `-uci` is used, whole directories may be deleted and marked with D:.

Caution should be exercised when using the `-c` option. Any files that may have been inadvertently deleted from indir will then be removed from the archive as well. For added safety, option `-c` is deliberately made explicit and separate from `-u`. Thus using `-u` alone is the *cautious updating mode*, which never deletes anything from the archive. However, it will still overwrite existing files with their new, possibly erroneous, contents. Beware that this can cause loss of previously useful content. Of course, this is true in general, whenever changing any files, by any means.

As seen above, options can be run together, e.g.:

```bash
ncrpt -xbuc indir keydir outdir
```

This will recursively update and clean the entire archive dir, so that it is as if freshly created from the current state of indir (with only -xb). However, as it is only incremental, it runs faster. This is convenient for backing up purposes.

There is a vulnerability inherent in `ncrpt` output directories. Specifically, specialist search engines sifting through the whole of the internet, matching up pairs of (keydir outdir) directories by their same structures, file names and sizes, could in theory pair them up, even if they were uploaded to two unrelated places. Using `pack` is therefore more secure for external storage.

It is recommended that `pack` be used prior to exporting snapshots of the local directory to any unsecure locations, such as the internet.

The only reason to ever prefer `ncrpt` over `pack` is when there is a lot of frequent updating being done. This is because `ncrpt` maintains the directory structure. Therefore it does not have to recreate the entire archive to update an individual file and it is not tied to sequential processing. It does fully utilise all the host computer cores for overall faster parallel execution.

### `dcrpt -[h][i][q][v] infile keyfile outdir`

Decrypts, unpacks, and decompresses directories created by `ncrpt`. Both of its first two input arguments must be directories. Its operations are carried out in exactly reverse order. It reads from `indir` the encrypted files. It also reads their associated keys from `keydir`. Individual files in input directories are paired up by their names, so never rename an encrypted file, unless you rename its corresponding key file as well. The root filenames in both directories must match. This issue does not arise with flat archive files produced by `pack`. 

Following decryption, the relevant decompression method(s) are applied to each file, so that the original files are exactly reconstructed in `outdir`. Please note that any strange types of system files or symbolic links might not be saved. Only genuine files that respond `true` to `bash` `-f` or genuine directories that respond `true` to `-d` test are saved.

### `pack [options] indir indexfile outfile keyfile`

Pack creates a brand new archive. Therefore the updating and cleaning options `-uc` are no longer available. Other `ncrpt` options remain. The most important difference is that instead of creating two output directories, `pack` creates three flat output files. The archived subdirectory and file names in `indexfile` are also compressed and encrypted.

All the keys for the whole archive now form a single sequential `keyfile`. No filenames are being duplicated or extra file extensions appended. This is overall a cleaner solution. The individual compressed files are also spliced together into a single `outfile`. This results in an even better overall compression.

The total of nine different combinations of compressions can be used, as with `ncrpt`, depending on each individual input file and its compressibility properties.

There is a price to be paid in terms of the execution time. The entire input directory tree structure now has to be traversed sequentially to maintain the same order, thus reducing the opportunities for parallel execution (that are fully exploited by `ncrpt`).

#### Simple Typical Usage

* `pack -xb indir indexfile outfile keyfile`
* upload the output files to three different places on the internet
* delete everything locally, intentionally or by accident:  
`rm -rf indir indexfile outfile keyfile`  
* at any time (and place) later, download the files back and then:
* `unpack indexfile outfile keyfile`

This will restore the original state of `indir`, at the originally specified writeable path, relative to the current directory.

### `unpack [options] indexfile outfile keyfile`

Is the inverse of `pack`. It decrypts, selects the right decompression methods, decompresses all files, and restores the exact contents of the original directory, with the same names of directories and files.

## Appendix - Background Scripts and Programs (not needed by the user)

`xorfork` (rust executable)  
The 'workhorse' of the encryption. 
Reads `stdin`, writes encrypted data to `outfile` and generated random key data to `stdout`.

`symcrypt` (rust executable)  
The inverse of `xorfork`. Reads `stdin` and random `keyfile`, writes decrypted data to `stdout`. Applies fast symmetric XOR decryption (or encryption). The two inputs of the same lengths are `XOR`ed together. This is a symmetric operation, so the two input sources may be swapped without affecting the result.

`hexcheck` (rust executable)  
is invoked by `pack -x`. It recognises hexadecimal (token) files and packs them to binary, which exactly halves them in size. Hexadecimal files should be an even number of bytes long and only contain (0-9,a-f) `ascii` characters. There are a few allowed  exceptions: upper case A-F are accepted but when converted back, they will always end up in lower case. Spaces and newlines just get deleted. This tolerant policy may result in some differences being reported between the original and the reconstructed files. Then it is best to replace the original file with its cleaned up, reconstructed version.

`hexify` (rust executable)  
is invoked by `unpack` to unpack the binary files back to their original hexadecimal form. In other words, it carries out an inverse operation to `hexcheck` above.

`base64` (linux utility)  
is invoked by `pack -b`. It recognises base64 files, resulting in 25% size reduction before the final general compression. Base64 files should not contain any non base64 characters, such as newlines, otherwise this strict test will reject them. This utility is also deployed in inverse mode by `unpack`.

`lzma` or `zstd` (linux utilities)  
are general compression methods used here for the final compression, as long as it result in size reduction. This is not necessarily the case for small and/or binary files. Any incompressible files are detected and encrypted as they are, even if their extensions are not on the 'do not compress' list. However, for extensions on the list, this test compression is avoided.

`keygen file > key` (bash script)  
writes to stdout random binary data of the same length as the given file. Now superseded by `xorfork` getting this data internally (in Rust).

`hexgen size` (bash script)  
writes to stdout `size` bytes of random hexadecimal data. It is only useful for generating test data.

`b64gen size` (bash script)  
writes to stdout `size` bytes of random base64 data. It is only useful for generating test data.

## Testing

An automated github action compiles the Rust programs and runs both `crptest`  and `packtest` over example `testing` directory included in the repository.
It tests all the main types of files: hexadecimal, base64, plain text and binary. It also tests recursive descent into a subdirectory. The 'test' badge at the top of this document lights up green when all the tests were passed. Note that only the summary output `test.log` is saved in the repository, everything else is tidied up (removed) after the tests.

Some character differences may arise for hexadecimal files because `hexcheck` converts both a-f and A-F to 10-15 and also it cleans up spurious spaces and newlines, instead of just rejecting such files. API keys should be separated into their own unique files. If the spaces/newlines turned out to be an unintended corruption, then the original file ought to be replaced by the cleaned up (reconstructed) version.

### `crptest testdir`

performs an automated test of `ncrpt`/`dcrpt`, creating directories, checking that not a single byte was corrupted anywhere, while encrypting and decrypting back the contents of (any) `testdir`. The reported compression/decompression rates and sizes should also exactly match. Afterwards it cleans up all the created directories.

### `packtest testdir`

is just like `crptest`, except it tests `pack`/`unpack` and the created output archive files instead of directories. The reconstructed files should be reported as being identical to the originals.

The reported overall compression rates depend on the compressibility of all the files.

## Exercise

Suppose you maintain some git repository, say `mygitrepo`. Go one level up:  `cd ..`, and run:

`ncrpt -xb mygitrepo mygitkeys gencrypted`  
Then, after some new trivial push from your repository, go back and run again:  
`ncrpt -xbuc mygitrepo mygitkeys gencrypted`  
This will update only the file(s) that have changed. What may come as a surprise is the number of files that were added/changed by git in `.git` directory.

Note that TokenCrypt does not leave any such large hidden footprints on your filesystem.

## Releases Log (the latest first)

**22-Oct-22** - Cleaned up, mostly `ncrpt` and `dcrpt`.

**21-Oct-22** - 'Windowsy' names of files and directories (with internal spaces) are now processed correctly, opening the possibility of using TokenCrypt even on Windows. Simplification: `dcrpt` is no longer reading old style file archives as well as the encrypted directories. Tar is no longer needed at all.

**20-Oct-22** - Release 1.1.0 Even safer and more compact. Fixed a bug in `xorfork` and swapped its outputs, so it now writes output into `outfile` and key data to `stdout`. Converted `pack` and `unpack` to use three archive files for increased security.

**19-Oct-22** - Release 1.0.8. More 'rustification'! New `xorfork` now encrypts and generates the key file all in one, which allowed some streamlining and simplification of `pack` and `unpack` scripts.

**18-Oct-22** - Release 1.0.7. As a precaution against losing subdirectories, recursive descent is now deployed by default. Option `-i` or `--ignore` now replaces it with the opposite effect: to create just a flat archive of the top level files.

**16-Oct-22** - Release 1.0.6. Removed the dependence on `tar`. Further improved speed and security. New simplified export/import scripts `pack` and `unpack` replace `expcrypt` and `impcrypt` respectively. `impcrypt` is renamed to `dcrpt`. It maintains backwards compatibility with archive files created by `expcrypt`. (It is also to be used for local directory maintenance, as the opposite of `ncrpt`).

**5-Oct-22** - Release 1.0.5. Removed the long deprecated `dcrpt` script. Rewrote all three C programs in Rust, thus increasing security. Changed github actions to work with Rust instead of previous C.

**19-Dec-21** - Release 1.0.3. Some code tidying and minor simplifications. No change in functionality.

## References and Further Information

1. [Bash for Fun](https://www.lulu.com/shop/libor-spacek/bash-for-fun/ebook/product-nerj22.html?q=&page=1&pageSize=4)  
Some of the techniques used in TokenCrypt
are explained (amongst other things) in this book on Bash programming. 

1. There is a blog [On Encryption and E-Democracy](https://oldmill.cz/2020-06-10-crypt.html) that describes in plain English the properties of XOR encryption and some interesting applications, primarily a model proposal for safe E-Democracy.
  
1. Blog [Multithreading Automation](https://oldmill.cz/2021-11-24-joy-of-bashing2.html) describing the scheduling of the background tasks within this project.
