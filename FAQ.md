## Frequently Asked Questions (FAQ)

**What if `crptest` reports missing NL and/or spaces in hex files?**

There may occasionally be reports of some missing spaces and newlines in
the reconstructed hexadecimal files.
This is because `hexcheck` tolerates them but skips them silently, as they are
not hexadecimal characters ( `xxd` does the same ).
However, note that, unlike `xxd`, `hexcheck` fails on any other non-hex
character and such file will not be treated as hexadecimal.
See the next question below.

The problem is that a file could consist entirely of non-printable
characters. Skipping them all and declaring such an empty file to be hexadecimal would
be a bug. Even when mixed half and half, at what point do we say that it is
now a genuine hexadecimal file? That is why the `hexcheck` has to be quite strict.

When this happens, it is a useful reminder to remove all white noise
from your original hex files. The spaces and newlines can be removed simply
by running `ncrpt` and `dcrpt` and then substituting the original hexadecimal files with their
reconstructed cleaned-up equivalents. However, any other spurious
characters will have to be removed manually.

**Why are my hex files over half their size after compression?**

They should normally be around 50% or less. If you have a whole directory
full of genuine hexadecimal files, the overall compression will be quite dramatic.

Any spurious non-hexadecimal characters, even just one of them,
will make `hexcheck` report the file as non-hexadecimal and its standard compression will then
be limited. It may be worth checking that what you thought were hexadecimal files
were actually accepted as such. This will be indicated by their keys in keydir
having a `.hex` extension added to their names. If the original file already had a .hex extension, it should now have two.

**Why does `crptest` report differences between some upper and lower case letters in hexadecimal files?**

Given that  A-F and a-f characters in hex files are both allowed, there is no way of recalling
which were which. For consistency, they all get converted to lower case.
Again, you can replace the original file which had some occurences of A-F with its reconstructed version to
prevent the repetition of these reports.

**How to get more hexadecimal files accepted?**

When a token consist of several parts, perhaps separated by a dash or some
other intervening non-hexadecimal characters, it is best to split it manually into individual
files, each containing only pure hexadecimal.

**What happens to impure 'hexadecimal' files?**

Impure 'hexadecimal' files/tokens still containing any non-hex characters
will be compressed and encrypted as they are, just like any other files.
Note that this is sub-optimal. It may be a pessimisation of security as well.

**What if `crptest` reports other differences?**

Should it list any other differences, then something has gone wrong. It will most
likely be some problem with the directories structure or accessability.
The original files in indir are always kept.
Should you discover a real bug, please create an issue at GitHub.

**When can the original files be deleted?**

They are left untouched in indir and it is up to you
not to lose them. Do not start deleting any of the originals until you 
are satisfied that the testing was successful, you have encrypted them, and double-checked manually that the encrypted files and keys exist and are of a reasonable non-zero size. 

Note that if you call crptest by mistake instead of ncrpt, the encrypted files will
be deleted afterwards. So do check that they actually exist.

**How does `dcrpt` know the actual method of compression that was used on any given file?**

Good question! As we have seen, hex compression and/or `lzma` or `zstd` compressions may or may not
be applied to any given file, depending on what is the user selected compressor, whether the
file is hexadecimal or not, whether it is actually reduced by compression or not, etc.
Actual compression(s) carried out are recorded in the extension(s) appended to the filename
of its key in keydir. (While the filenames of the encrypted files are left
exactly the same as those of the original files).

Be careful not to interfere with the keys' extensions, as this would prevent
successful decompression. As would changing any of the keys' filenames in general.

**What is the biggest hazard in using TokenCrypt?**

Misplacing, corrupting, failing to update the backup of, or accidentally
overwriting the keys in keydir and encrypted files in outdir. The keys directory should be kept somewhere separate from outdir for security reasons but the rub is that this makes them  easier to lose.
You need to be well organised with your file backups (in two different places)
and the directories correspondences across them.
