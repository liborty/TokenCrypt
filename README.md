# TokenCrypt

**A simple utility to encrypt and decrypt directories of security tokens and any other files.
High security without onerous complications.**

## Version 1.1.2  [![Actions Status](https://github.com/liborty/TokenCrypt/workflows/compilation/badge.svg)](https://github.com/liborty/TokenCrypt/actions) [![Actions Status](https://github.com/liborty/TokenCrypt/workflows/test/badge.svg)](https://github.com/liborty/TokenCrypt/actions)

## Outline

Use at your own risk! No warranties are given or implied. 
When using this software, you agree to apply it to ethical purposes.

Internet security tokens usually consist of 32,64 or more hexadecimal characters. 
They are increasingly used to facilitate secure access over the internet protocols
to various Applications Programming Interfaces (APIs). Being plain text,
they are easily transmitted but they need to be stored securely,
that means encrypted. 

TokenCrypt takes whole directories of tokens
and/or any other files, recognises hexadecimal data files, chooses the best compression 
process individually for each file and finally encrypts them all with extremely high security.

There may be files and directories, perhaps even whole databases of security tokens,
all mixed up with other types of files. This is perhaps not the best practice but 
it may well arise and could involve much work to separate them. `TokenCrypt` 
copes with such directories automagically.

The main workhorse at the heart of TokenCrypt is `symcrypt.c`, which applies fast symmetric
encryption or decryption to any type of file of any length, while using practically no memory.

Then there are two bash scripts to automate the encryption and decryption,
subsuming the tasks of compression and keys generation. There is also an automated overall testing script.

The hexadecimal (token) files are recognised and converted to binary, 
which halves them in size.

Then modern zstd compression is applied but only if it actually reduces the size of the file.
This is generally not going to be the case for small and/or binary files. 
`TokenCrypt` finally chooses to encrypt the shortest form of each file.

Decryption is the inverse of this process. See the scripts `ncrpt` and `dcrpt` for details.
However, knowledge of the algorithm is not necessary for effective TokenCrypt use.

The entire process of encryption and decryption is automatically
tested using the script `crptest`.

## Installation

Needs to be repeated every time that an updated repository is dowloaded or pulled.
(Except when the programs and scripts are unchanged).
This software was tested under Linux. Installation from source needs just a C compiler,
either clang or gcc. Download or clone this directory, cd into it and then:  

**`make CC=clang`**

or if clang is not installed, just use the default compiler 
(under Linux it is usually gcc): **`make`**

When you are using a typical Linux, you can often skip the compilation step entirely. 
Then the pre-compiled binaries `symcrypt` and `hexcheck` included in this repository
will be installed by default.

**`sudo ./install`**
 
Copies all the executables for system-wide use into /usr/local/bin.  
To remove them again, use: **`sudo ./uninstall`**

Alternatively, you can copy them manually to any of your own `bin` 
directories in your path and this does not require `sudo` privileges, e.g.:
**`cp symcrypt hexcheck ncrpt dcrpt crptest ~/bin`**

Automated action: 'compile' runs '`make CC=clang`' at GitHub. 
The 'compile' badge at the top of this document lights up green when the build 
that automatically generated the pre-compiled binaries was successful.

### Dependencies

Standard hex-dump utility **`xxd`** which is normally pre-installed. If not, usually
'`sudo apt-get install xxd`' will install it.
  
**`zstd`** compression which may need installing with: '`sudo apt-get install zstd`'.
This can be done either before the above installation or at any time thereafter.

Should you prefer `lzma` compression, as in some of
the earlier versions, you can change it back in `ncrpt` and `dcrpt` scripts,
not forgetting to change the extension names from '`.zst`' to '`.lz`'.
If there is demand for using different compressors, this can be automated in later
versions of `TokenCrypt`.

## Usage

**`ncrpt`** path/dirname

Encrypted output files go into `./dirname_crp` (under the current directory).
Unique new key is generated for each file in `path/dirname` and
written to `./dirname_key` that exactly mirrors `./dirname_crp` (and `path/dirname`).
Reports input and output directories sizes (in bytes), the overall compression 
and the number of files encrypted.

Does not work recursively. It only encrypts the files at the top level of `path/dirname`
and ignores any subdirectories. (Recursive version may be coming in a later release).

**`dcrpt`** path/dirname_key path/dirname_crp

is the inverse of `ncrpt`. It uses the keys in  `path/dirname_key` to decrypt 
their name corresponding files in `path/dirname_crp`.
All the decrypted results are written into `./dirname_org` (under the current directory).

## Testing

**`crptest`** path/dirname 

Automatically tests `ncrpt` and `dcrpt`. It first encrypts and then decrypts back again
all the files in the given input directory and compares the results against the original files.
It cleans up after itself except for the keys directory `./dirname_key`,
which is left for reassurance and for information about how was each test file compressed. 
There should be only a blank produced after "crptest found these differences:"

Automated github action: 'test' runs `crptest` over `testing` directory that is
included in the repository.
It tests all the main types of files: hexadecimal, text and binary. 
The 'test' badge at the top of this document lights up green 
when the test was successful. Note that only the output `test.log`
is saved in the repository from this automatic test. Here is what it looks like:

	crptest on: Sat 4 Jan 11:25:45 UTC 1020
	Output size:	6220 (testing_crp)
	Original size:	8766 (testing)
	Compressed to:	70.95%
	ncrpt: encrypted 3 files into testing_crp, keys are in testing_key
	dcrpt: decrypted 3 files into testing_org, 0 failures
	crptest found these differences:
	crptest tested 3 files

## Frequently Asked Questions (FAQ)

**What if `crptest` reports missing NL and/or spaces in hex files?**

There may occasionally be reports of some missing spaces and newlines in
the reconstructed hexadecimal files.
This is because `hexcheck` tolerates them but skips them silently, as they are
not hexadecimal characters ( `xxd` does the same ). 
However, note that, unlike `xxd`, `hexcheck` fails over any other non-hex 
character and such file will not be treated as hexadecimal.
See the next question below.

The problem is that a binary file could consist entirely of non-printable
characters. Skipping them all and declaring such an empty file to be hexadecimal would
be a bug. Even when mixed half and half, at what point do we say that it is
now a genuine hexadecimal file? That is why the hexadecimal check has to be quite strict.

When this report happens, it is a useful reminder to remove the white noise 
from your original hex files. The spaces and newlines can be removed simply
by running `ncrpt` and `dcrpt` and then substituting the original hexadecimal files with their
reconstructed cleaned-up equivalents in `./dirname_org`. However, any other spurious
characters will have to be cleaned up manually.

**Why are my hex files over half their size after compression?**

They should normally be 50% or less. If you have a whole directory
full of genuine hexadecimal files, the overall compression will be quite dramatic.

Any spurious white noise or other non-hexadecimal characters, even just one of them,
will make `hexcheck` report the file as non-hexadecimal and its compression will
be limited. It may be worth checking that what you thought were hexadecimal files
were actually accepted as such. This will be indicated by their keys in `./dirname_key`
having a `.hex` extension in their names. If the original file already had a .hex extension,
it should now have two.

**Why does `crptest` report differences between some upper and lower case letters?**
 
This is the expected behaviour. A-F letters in hex files are intentionally changed
by `hexcheck` into their lower case equivalents a-f (standard hexadecimal form).
Again, you can replace the original file with its reconstructed version to
prevent the repetition of these reports.

**How to get more hexadecimal files accepted?**

When a token consist of several parts, perhaps separated by a dash or some
other intervening non-hexadecimal characters, it is best to split it manually into individual
files, each containing only pure hexadecimal. Any remaining spaces and newlines 
will be accepted but note that they will be deleted in the process.

**What happens to impure 'hexadecimal' files?**

Impure 'hexadecimal' files/tokens still containing any non-hex characters
will be compressed and encrypted as they are, just 
like any other files. Note this is sub-optimal, as they can end up twice as
big as they needed to be. It may be a pessimisation of security as well.

**What if `crptest` reports other differences?**

Should it list any other differences, then something has gone wrong. It will most
likely be some problem with the directories structure or accessability.
The original files in `path/dirname` will be untouched.
Should you discover a real bug, please create an issue at GitHub.

**When can the original files be deleted?**

These are left untouched in their original `path/dirname` and it is up to you
not to lose them. Do not start deleting any of the originals until you 
are satisfied that the testing was successful, you have invoked `ncrpt path/dirname` 
to encrypt for real, and double-checked manually that the encrypted files and keys exist
and are of a reasonable non-zero size. The output of ncrpt reports the overall size.

**How does `dcrpt` know the actual method of compression that was used on any given file?**

Good question! As we have seen, hex compression and/or zstd compression may or may not
be applied to any given file, depending on what is the most effective method for it.
This information is recorded in the extension appended to the filename
of its key in `./dirname_key`. (While the filenames of the encrypted files are left 
exactly the same as those of the original files).

Be careful not to interfere with the keys' extensions, as this would prevent
successful decryption. As would changing any of the keys' filenames in general.

**What is the biggest hazard in using TokenCrypt?**

Misplacing, corrupting, failing to update the backup of, or accidentally
overwriting the keys in the generated `./dirname_key` directory.
In two short words: losing them. 

The keys directory should be kept somewhere separate from 
`./dirname_crp` for security reasons but the rub is that this makes it easier to lose.
You need to be well organised with your file backups (in two different places)
and the directories correspondences across them.
