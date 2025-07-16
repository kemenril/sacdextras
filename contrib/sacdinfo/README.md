## sacdinfo - a program using libsacd to dump information from SACD files

This software is a simple utility to query track information (artist, title, channels, ...) from SACD audio files.  It relies upon *libsacd*, from the [SACD](https://github.com/Sound-Linux-More/sacd) utility, which is available as part of the sound-linux-more package.  For simplicity's sake, it makes use of the library and is also distributed under the same GPL 3 license as the SACD utility.

You may dump information from any set of one or more files in either human-readable text form, or a JSON format similar to the one used in Andrew Radav's [ID3-JSON](https://github.com/AndrewRadev/id3-json).  In either case, output can be written to the terminal, or to a file.

### Build

The Makefile should be sufficient.  There are two interesting targets, one for *sacdextras*, and one for *shared*.  The former links statically with the object files generated in the man *sacd* build in the parent directory.  The latter uses the share library generated when you *make shared* up there.

### Usage
```
   sacdinfo [-o outfile] [-j] [-s SECTION] [-t TRACK] <file> [file] [file...]
       -o, --output       : Write the output to a given file.
       -j, --json         : Format the output as JSON, instead of plain text.
       -s, --section      : Read one of "[S]tereo" or "[M]ultichannel" areas.
       -t, --track        : Look up this single track. (Requires you to
                                specify a section, and only a single file)
       -h, --help         : Show this message
   Note that track and section may be left out, and the default is toread everything.
```


