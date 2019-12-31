# TokenCrypt

**A simple utility to encrypt and decrypt directories of security tokens and any other files.
High security without onerous complications.**

## Version 1.1.0  

## Outline

Internet security tokens are usually 32,64 or more hexadecimal characters. 
They are increasingly used to facilitate secure access to various 
Applications Programming Interfaces (APIs). Being plain text,
they are easily transmitted over the internet but they need to be stored securely,
that means encrypted. 

TokenCrypt takes whole directories of tokens
and/or any other files, recognises hexadecimal data, applies the best compression 
process individually to each file and then encrypts them all with extremely high security.

There may be files and directories, perhaps even whole databases of security tokens,
all mixed up with other types of files. This is perhaps not the best practice but 
it may well arise and could involve much work to separate them. TokenCrypt copes with it
automagically: `hexcheck.c` detects entirely hexadecimal files.

The main workhorse of TokenCrypt is `symcrypt.c` which does fast symmetric
encryption or decryption of any type of file of any length. It uses practically no memory.

Then there are two bash scripts to automate the encryption and decryption,
subsuming compression and keys generation. Plus an automated overall testing script.

The hexadecimal (token) files are automatically recognised and converted to binary, 
which halves them in length.

Then compression is tried and only if it results in a smaller file, its result is used.
(This will not generally be true for small and/or binary files).

Finally, the binary encryptor `symcrypt.c` is applied.

Decryption is the inverse of this process. See the scripts `ncrpt` and `dcrpt` for details.
However, the knowledge of the algorithm is not necessary for effective TokenCrypt use.

The entire process of encryption and decryption can be automatically
tested using the script `crptest`.

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

## Testing

**`crptest`** path/dirname 

Automatically tests `ncrpt` and `dcrpt`. It first encrypts and then decrypts back again all the files in the given directory
and compares the results against the original files. It cleans up after itself except for
the reconstructed files directory `./dirname_org`, left for double reassurance.

There should be just a blank after "crptest found these differences:"

## Frequently Asked Questions (FAQ)

**What if `crptest` reports missing NL and/or spaces in hex files?**

It may report some missing spaces and newlines in reconstructed hexadecimal files.
This is because `hexcheck` tries to be tolerant of them but `xxd` subsequently
quite rightly deletes them. When this happens, it is a useful reminder to remove them
from your original hex files. It can be done most simply by substituting the 
original hexadecimal files with their reconstructed cleaned-up equivalents from `./dirname_org`

**My hex files are achieving less than 50% compression?**

They should normally get at least 50% compressed.
Any other spurious white noise or other non-hexadecimal characters, even just one of them,
will make `hexcheck` report the file as non-hexadecimal and so its compression will
be limited. It may be worth checking that what you thought were hexadecimal files
were actually accepted as such. This will be indicated by their keys in `./dirname_key`
having a `.hex` extension in their names. If the original file already had a .hex extension,
it should now have two.

**How to get more hexadecimal files accepted?**

When a token consist of several parts, perhaps separated by a dash or some
other intervening non-hexadecimal characters, it is best to split it into individual
files, each containing only pure hexadecimal. Any remaining spaces and newlines 
will be accepted but note that they will be deleted in the process.

**What happens to impure hexadecimal files?**

It is possible and simple to naively encrypt/decrypt the hexadecimal files/tokens
as they are but this is sub-optimal, as they can end up twice as long. 
This is just what happens to such 'impure' hex files still containing some odd non-hex characters.
It may be pessimisation of security as well.

**What if `crptest` reports other differences?**

Should it list any other differences, then something has gone wrong. It will most
likely be some problem with the directories structure or accessability.
The original files in `path/dirname` will be untouched.

**When can I start deleting my original files?**

These are normally left untouched in their original `path/dirname` and it is up to you
to not lose them. You should not start deleting any of the originals until you 
are satisfied that the testing was successful, you have invoked `ncrpt path/dirname` 
to encrypt for real, and double-checked manually that the encrypted files and keys exist
and have reasonable lengths.

**What is the biggest hazard of TokenCrypt?**

Misplacing, corrupting, failing to update the backup of, or accidentally overwriting the keys in the generated
`./dirname_key` directory. In two short words: losing them. The keys directory should be kept somewhere separate from 
`./dirname_crp` for security reasons but the rub is that this makes it easier to lose.
You need to be well organised with your file backups.

## Conclusion

Watch this space, more facilities are in preparation.
