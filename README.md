# TokenCrypt [![GitHub last commit](https://img.shields.io/github/last-commit/liborty/tokencrypt/HEAD?logo=github)](https://github.com/liborty/tokencrypt) [![Actions Status](https://github.com/liborty/tokencrypt/actions/workflows/test.yml/badge.svg)](https://github.com/liborty/tokencrypt/actions)

**Efficiently compress, securely encrypt and archive directories trees. High security combined with ease of use.**

**Disclaimer:** use at your own risk! No warranties are given or implied. Backwards compatibility is not guaranteed!
By downloading this software, you agree not to use it for unethical purposes.

## Practical Security Advice

`TokenCrypt` uses theoretically unbreakable symmetric encryption. However, the following precautions should be followed:

Run it only on systems where `/dev/urandom` returns truly random bytes. Better not use it for large archives immediately after reboot, as the operating system may not have yet gathered enough physical randomness from user interactions. 'Headless' servers may be problematic in this regard.

An input directory is compressed and encrypted by `ncrpt` into two separate output directories, with user specified names. The original directory can be restored from them by `dcrpt`. These two scripts are suitable for local archive maintenance only.

For exports, use `pack` (`unpack`), which archives (restores) the directory tree to (from) three flat archive files. This is more compact and significantly more secure in transit and storage.

Should one of these files be lost or the implicit association between them be forgotten, then it will not be possible to reconstruct the original data! Therefore, it is recommended to record the chosen output names and to store them in a safe place, before ever deleting the original data.

The encrypted archive files are just meaningless 'random' data and thus can be stored anywhere, even 'on the cloud'. The critical part is to prevent a potential eavesdropper from matching up these three files, as that is the only way to decrypt them. That is why they are not given related names automatically. Normally, the user should choose unrelated paths/names for them and keep them well separated. (As with private keys in asymmetric encryption).

As of `v1.1.0`, it is now much harder to guess the right combination of these files, all three being of different sizes, with different names and, hopefully, kept in different places.

As a bare minimum, the `keyfile` (the last argument to `pack` and `unpack`), should be well separated from `indexfile` and `outfile`. It can be safely uploaded, on its own, to any 'cloud' location.

Important directories can be tested first with `packtest` before proceeding. Gigabytes may take some minutes to process, so just run the script in the background while you get on with other things. It is still faster than many other archiving and backup tools.

For a complete peace of mind, save locally the source that was used for the successful `packtest`, in case anything happens to this repository in the future.

## Introduction

It is not the objective of TokenCrypt to replace `git` and to keep the complete histories of everything, as that carries significant costs in complexity and storage. The quest for being totally foolproof is subject to the law of diminishing returns, as human foolishness generally knows no bounds. TokenCrypt is more akin to `borg`: an efficient archiver and backing up tool. The key difference, excuse the pun, lies in TokenCrypt deploying more secure encryption and somewhat better compression as well. In its update mode, it only maintains the current version of the archive. Historical checkpoints are up to the user to create and keep, as and when required.

Scripts `pack` and `ncrpt` read given input directory tree, containing any types of files. `ncrpt` creates two output directories, `pack` creates three flat archive files.

Some already compressed files afford only minor further compression, if any. To save time, they are recognised simply by their well known extensions: `jpg, jpeg, mp4, zip, 7z, lz, zst, gz, tgz, bz2`, and their upper case versions. Files with these specific extensions are encrypted uncompressed.

Given `-xb` options, `ncrpt` and `pack` scripts will recognise hexadecimal files and base64 files respectively and convert them to more compact binary data. For hexadecimal files, this results in 50% compression. They then individually select the best final compression method for each file and securely encrypt it.

Note: internet security tokens usually consist of 32, 64 or more hexadecimal characters. They are increasingly used to facilitate secure access over the internet protocols to various Applications Programming Interfaces (APIs). Even git uses hexadecimal 'slugs'. Being plain text, they are easily transmitted but they should be stored securely, i.e. strongly encrypted. Base64 encoding into printable characters is also generally used to encode binary data, for similar reasons.

Hexadecimal security tokens, base64 files and all other types of files may sometimes occur in in the same directory. This is not the best practice but it does arise. It could involve much work trying to separate the files. `ncrpt` and `pack` cope with such mixed contents directories by applying individual data type recognition tests to each file. In this case, they do not rely on extension names, as that could lead to errors.

## Installation

This software was developed and tested under Linux and its x86-64  binaries are available in the repository release. Compilation from source needs Rust installed. Download or clone this repository (to get the latest version of the bash scripts) and `cd` into it.

    ./install

will then locally compile four Rust  programs `xorfork`, `symcrypt`, `hexify` and `hexcheck`. The script then asks to raise `sudo` privileges and installs everything that may have changed into `/usr/local/bin` (including the Bash scripts).

The binaries provided in the repository can be used to avoid Rust installation and compilation. Rust sources are automatically compiled and tested by `github.com` actions - see the test badge at the top of this document. The executables downloaded from the repository release should be copied manually into `/usr/local/bin` for system-wide use. Alternatively, they (and the scripts), can be copied to any other `bin` directory in the search path, without using `sudo` privileges. This step should be repeated whenever some programs and scripts have been updated.

Reasons for local compilation may be:

1) when the binaries may have been compromised
2) when using a different machine architecture.

### `sudo ./uninstall`

will delete previously installed programs and scripts from `/usr/local/bin`. It can be used to enforce a completely clean restart.

## Dependencies

`/dev/urandom` - standard source of truly random data.

`du` (disk usage) - standard tool which is part of Linux.

`base64` tool which is also normally pre-installed on Linux.

`lzma` is the default compression utility. It is normally pre-installed. If not, install it with: `sudo apt-get install lzma`
  
`zstd` is an alternative compression, selectable with `-z` option to `ncrpt` and `pack`. It usually needs to be installed with: `sudo apt-get install zstd`

`dcrpt` and `unpack` issue warnings if any of the necessary utilities are not installed. It is probably best to install them all. There is not much difference between `lzma` and `zstd` compressions but they each have their own fans. `lzma` appears to have slightly better compression rate and `zstd` is slightly faster and more controllable. Of course, to unpack archives on a new machine, it must have the same utility installed, too.

## Usage

There are four command line interface (CLI) bash scripts that do all the work and automate the whole process:

### `ncrpt [options] indir keydir outdir`

This script performs data type analysis, optimal compression selection, compression, key generation, key saving and encryption. Also recursive archiving and subsequent archive maintenance.

Long options introduced by `--` are also recognised.  
The options explained:

    -b --b64 test for base64 files, 
    -c --clean up existing archive and its keys,
    -e --exclude (globally) listed file extensions,
    -h --help,
    -i --ignore (globally) named subdirectories,
    -q --quiet, suppress the final report,
    -r --recurse into subdirectories,
    -u --update existing archive and its keys,
    -v --verbose information on compressing each file
    -x --hex test for hexadecimal files, 
    -z --zstd compression to be used instead of lzma.     

- When `-u` option is used, then instead of creating a new archive, an existing one is compared to the latest state of indir and updated.  
- Option `-c` only makes sense as `-uc` because a new archive needs no cleaning.  
- Option `-i` only makes sense as `-ri` because without `-r` all subdirectories are ignored anyway by default. It requires a quoted list of space separated directory names.
- Option `-e` requires quoted list of space separated extensions in lower case. Files with upper case extensions will also be recognised and ignored.  
- Option `-ur -e '...' -i '...' (updating only) will not update excluded file extensions and ignored directories and their old versions will remain.  
- Options `-ucr -e '...' -i '...' (cleaning) will delete files/directories from the archive for two possible reasons: a) they had been deleted from INDIR or b) they had been specified on the exclude/ignore lists. After the cleaning, the remaining contents will be updated.

As seen above, options can be run together, except following `-e` and `-i`, which must both be followed by a quoted, space separated list. Long lists of exceptions will cause some deterioration of speed.

The tests for hexadecimal `-x` and base64 `-b` files should only be specified when the input directory likely contains such files. However, these tests are fast. They usually terminate after reading only the first few bytes. When these options are omitted by mistake, then everything will still work. However, the default compression of these types of files will take up more space than was strictly necessary.

The three arguments are mandatory: the input directory, the keys directory and the encrypted directory. Both output directories (`keydir` and `outdir`) will mirror `indir` in their structure and file names; `keydir` will hold the keys and `outdir` will hold the encrypted files.

The summary at the end, such as the one shown in `test.log`, reports the sizes (in bytes) of input and output directories and the total compressed size as a percentage of the original size. Compression to 50% will compensate for the creation of the encryption keys. Sometimes even better compressions may be achieved.

Option `-q` (`--quiet`) cancels the final report.
Option `-v` (`--verbose`) adds details of compressing each file. Selecting  both options `-vq`, contradictory as it may seem, turns on the individual file reports and turns off the final summary. The encryption itself is so unproblematic that it does not require any reports.

Once a directory has been compressed and encrypted, it is subsequently possible to update the keys and outdir directories (the archives) with option `-u`. This will add any new files and recode updated ones. Added new files are marked with a: and more recent existing files are updated (marked with u:). The same actions will be applied to all subdirectories, unless option `-i` (ignore subdirectories) has been deployed. Capital letters A:,U: denote these two operations when applied to whole subdirectories.

An archive can additionally be cleaned up with `-uc`, to ensure that a new state of the input directory and its archives match. That is, files no longer existing in indir will be deleted from the archive (and marked with d:). When `-r` is added, whole directories may be deleted and marked with D:.

Caution should be exercised when using the `-c` option. Any files that may have been inadvertently deleted from indir will be removed from the archive as well. Option `-c` is explicit and separate from `-u` for added safety. Thus using `-u` alone is the *cautious updating mode*, which never deletes anything from the archive. However, it will still overwrite existing files with their new, possibly erroneous, contents. This can cause loss of previously useful content. Of course, this is true in general, whenever updating any files, by any means.

There is a vulnerability inherent in `ncrpt` output directories. Specifically, specialist search engines sifting through the internet, matching up pairs of (keydir outdir) directories by their same structures, file names and sizes, could in theory pair them up, even if they were uploaded to two unrelated places. Using `pack` is therefore more secure for external storage.

It is recommended that `pack` be used prior to exporting snapshots of the local directory to any unsecure locations, such as the internet.

The only reason to ever prefer `ncrpt` over `pack` is when there is a lot of frequent updating being done. This is because `ncrpt` maintains the directory structure. Therefore it does not have to recreate the entire archive to update an individual file and it is not tied to sequential processing. It does fully utilise all the host's cores for faster parallel execution.

### `dcrpt -[h][i][q][v] indir keydir outdir`

Decrypts, unpacks, and decompresses directories created by `ncrpt`. Both of its first two input arguments must be directories. Its operations are carried out in exactly reverse order. It reads from `indir` the encrypted files. It also reads their matching keys from `keydir`. Individual files in input directories are paired up by their names, so never rename an encrypted file, unless you rename its corresponding key file as well. The root filenames in both directories must match. This does not apply to flat archive files produced by `pack`.

Following decryption, the relevant decompression method(s) are applied to each file, so that the original files are exactly reconstructed in new `outdir`. Please note that any strange types of system files or symbolic links might not be saved. Only genuine files that respond `true` to `bash` `-f` or genuine directories that respond `true` to `-d` test are saved.

### `pack [options] indir indexfile outfile keyfile`

Pack always creates a brand new archive. Therefore the updating and cleaning options `-uc` are no longer available. Other `ncrpt` options remain. The most important difference is that instead of creating two output directories, `pack` creates three flat output files. The archived subdirectory and file names in `indexfile` are also compressed and encrypted.

All the keys for the whole archive now form a single sequential `keyfile`. No filenames are being duplicated or extra file extensions appended. This is overall a cleaner solution. The individual compressed files are also spliced together into a single `outfile`. This results in an even better overall compression.

The total of nine different combinations of compressions can be applied, as with `ncrpt`, depending on each individual input file and its compressibility properties.

There is a price to be paid in execution time. The entire input directory tree structure now has to be traversed sequentially to maintain the same order, thus preventing parallel execution (that is fully exploited by `ncrpt`).

#### Simple Typical Usage

* `pack -xbr indir indexfile outfile keyfile`
* upload the output files to three different places on the internet,
* delete everything locally, intentionally or by accident:  
`rm -rf indir indexfile outfile keyfile`  
* at any time (and place) later, download the files back and:
* `unpack indexfile outfile keyfile`

This will restore the original state of `indir`, at the originally specified writeable path, relative to the current directory.

### `unpack [options] indexfile outfile keyfile`

Is the inverse of `pack`. It decrypts, selects the right decompression methods, decompresses all files, and restores the exact contents of indir  in the current directory.

## Appendix - Background Scripts and Programs (not needed by the user)

`xorfork` (rust executable)  
The encryption engine: reads `stdin`, writes encrypted data to `outfile` and generated random key data to `stdout`.

`symcrypt` (rust executable)  
The inverse of `xorfork`. Reads `stdin` and random `keyfile`, writes decrypted data to `stdout`. Applies fast symmetric XOR decryption (or encryption). The two inputs of the same lengths are `XORed` together. This is a symmetric operation, so the two input sources may be swapped without affecting the result.

`hexcheck` (rust executable)  
is invoked by `pack -x`. It recognises hexadecimal (token) files and packs them to binary, which exactly halves them in size. Hexadecimal files should be an even number of bytes long and only contain (0-9,a-f) `ascii` characters. There are a few allowed  exceptions: upper case A-F are accepted but when converted back, they will always end up in lower case. Spaces and newlines just get deleted. This forgiving policy may result in some differences being reported between the original and the reconstructed files. Then it is best to replace the original file with its cleaned up, reconstructed version.

`hexify` (rust executable)  
is invoked by `unpack` to unpack the binary files back to their original hexadecimal form. In other words, it carries out the inverse operation to `hexcheck` above.

`base64` (linux utility)  
is invoked by `pack -b`. It recognises base64 files, resulting in 25% size reduction before the final general compression. Base64 files should not contain any non base64 characters, such as newlines, otherwise this strict test will reject them. This utility is also deployed in its inverse mode by `unpack`.

`lzma` or `zstd` (linux utilities)  
are general compression methods used here for the final compression, as long as it will result in size reduction. This is not necessarily the case for small and/or binary files. Any incompressible files are detected and encrypted as they are, even if their extensions are not on the 'do not compress' list. However, for extensions on the list, this test compression is avoided.

`keygen file > key` (bash script)  
writes to stdout random binary data of the same length as the given file. Now superseded by `xorfork` getting this data internally (in Rust).

`hexgen size` (bash script)  
writes to stdout `size` bytes of random hexadecimal data. It is only useful for generating test data.

`b64gen size` (bash script)  
writes to stdout `size` bytes of random base64 data. It is only useful for generating test data.

## Testing

An automated github action compiles the Rust programs and runs both `crptest`  and `packtest` over example `testing` directory included in the repository.
It tests all the main types of files: hexadecimal, base64, plain text and binary. It also tests recursive descent into a subdirectory. The 'test' badge at the top of this document lights up green when all the tests were passed. Note that only the summary output `test.log` is saved in the repository, everything else is tidied up (removed) after the tests.

Some character differences may sometimes arise for hexadecimal files because `hexcheck` converts both a-f and A-F to 10-15. Also, it deletes all spaces and newlines, instead of strictly rejecting such files. API keys should always be stored in files one at a time. Therefore any spaces/newlines separators are spurious characters. The original file can be replaced by the cleaned up (reconstructed) version, ensuring no more discrepancies in the future.

### `crptest testdir`

Tests `ncrpt`/`dcrpt`, creating directories, checking that not a single byte was corrupted anywhere, while recursively encrypting and decrypting back the entire contents of (any) `testdir`. Note that `--exclude` or `--ignore` options are not deployed in this test. Therefore the reported overall compression/decompression rates and sizes should exactly match. The overall compression rate depends on the compressibility of all the individual files. Afterwards, all the created directories are deleted.

### `packtest testdir`

is just like `crptest`, except it tests `pack`/`unpack` (the archive files instead of the directories). The reconstructed files should be reported as being identical to the originals.

## Examples

    ncrpt -xbri 'bin' sourcedir keysdir outdir

will test for hexadecimal and base64 files in sourcedir, recurse into all subdirectories except any named `bin`, efficiently compress and encrypt everything else, writing keys into keysdir and output archives into outdir.

    ncrpt -xbruci 'data' sourcedir keysdir outdir

will recursively update and clean the entire archive outdir, so that it is as if freshly created from the current state of sourcedir (with only -xbri). However, with `-u` it is incremental, so it runs faster. This is convenient for regular backing up purposes. 'bin' is no longer being ignored, so it will be added to the archive. Since `-c` is also given, any directories named 'data', that are now newly ignored, will be deleted from the archive.

## Exercise

Suppose you maintain some git repository, say `mygitrepo`. Go one level up:  `cd ..`, and run:

`ncrpt -xbr mygitrepo mygitkeys gencrypted`  
Then, after some new trivial push from your repository, go back and run again:  
`ncrpt -xbruc mygitrepo mygitkeys gencrypted`  
This will update only the file(s) that have changed. What may come as a surprise is the number of files that were added/changed by git within the `.git` directory.

Note that TokenCrypt does not leave any such large hidden footprints on your filesystem.

## Releases Log (the latest first)

**3-April-24** - `ncrpt` now does sensible things with the new options `-e -i` also in the updating and cleaning modes. Implemented these new options for `pack` as well, making `TokenCrypt` a more practical archiving tool.

**1-April-24** - `ncrpt` can now globally ignore named directories with, e.g. `-i 'bin test backup'` and to globally exclude all files with listed extensions, e.g. `-e 'exe dat bak'`. Note that `-i` was previously used to turn off recursion. Recursion is now off by default and must be activated explicitly with option `-r` (usual conservative convention).
 
**30-March-24** - Corrected some typos in `README.md`, updated `test.yml`, tested on `Rust 1.77.1`.

**23-Oct-22** - Release 1.1.1 Housekeeping release, encapsulating the changes to date.

**22-Oct-22** - Cleaned up, mostly `ncrpt` and `dcrpt`.

**21-Oct-22** - 'Windowsy' names of files and directories (with internal spaces) are now processed correctly, opening the possibility of using TokenCrypt even on Windows. Simplification: `dcrpt` is no longer reading old style file archives as well as the encrypted directories. `Tar` is no longer needed at all.

## References and Further Information

1. [Bash for Fun](https://www.lulu.com/shop/libor-spacek/bash-for-fun/ebook/product-nerj22.html?q=&page=1&pageSize=4)  
My book on Bash programming, with specific references to some of the techniques used in TokenCrypt.

1. There is a blog [On Encryption and E-Democracy](https://oldmill.cz/2020-06-10-crypt.html) that describes in plain English the properties of XOR encryption and some interesting applications, primarily a model proposal for safe E-Democracy.
  
1. Blog [Multithreading Automation](https://oldmill.cz/2021-11-24-joy-of-bashing2.html) describing the scheduling of the background tasks in this project (by `ncrpt` and `dcrpt`).
