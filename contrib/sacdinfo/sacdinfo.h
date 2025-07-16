#ifndef		__SACDTAGS_H
#define		__SACDTAGS_H

/* sacdinfo, a program using libsacd to dump information from SACD files
         v0.1
Copyright 2025 Christopher Smith <protheus@byteorder.net>

sacdinfo is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sacdinfo is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SACD.  If not, see <http://www.gnu.org/licenses/gpl-3.0.txt>.
*/


const char stHelpHeader[] =
	"\n"
	"Usage: ";

const char stHelpMessage[] =
        " [-o outfile] [-j] [-s SECTION] [-t TRACK] <file> [file] [file...]\n\n"
        "    -o, --output       : Write the output to a given file.\n"
        "    -j, --json         : Format the output as JSON, instead of plain text.\n"
        "    -s, --section      : Read one of \"[S]tereo\" or \"[M]ultichannel\" areas.\n"
        "    -t, --track        : Look up this single track. (Requires you to\n"
	"                         specify a section, and only a single file)\n"
        "    -h, --help         : Show this message\n\n"
        "  Note that track and section may be left out, and the default is toread everything.\n\n";

const struct option stGetOptions[] = {
	{"output",required_argument,NULL,'o'},
	{"json",no_argument,NULL,'j'},
	{"section",required_argument,NULL,'s'},
	{"track",required_argument,NULL,'t'},
	{"help",no_argument,NULL,'h'},
	{NULL,0,NULL,0}
};



#endif
