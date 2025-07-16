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

#include <string>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <libsacd/sacd_media.h>
#include <libsacd/sacd_reader.h>
#include <libsacd/sacd_disc.h>
#include <libsacd/sacd_dsdiff.h>
#include <libsacd/sacd_dsf.h>

#include "sacdinfo.h"


//Detect based on extension; pretty much what the main utility does.
media_type_t getMediaType(string inFile) {
	media_type_t mediaType = UNK_TYPE;
	string ext = inFile.substr(inFile.length()-4, 4);

        for(unsigned short c = 1;c < ext.length();c++) ext[c] = tolower(ext[c]);

        if (ext == ".iso" || ext == ".dat") {
            mediaType = ISO_TYPE;
        }
        else if (ext == ".dff") {
            mediaType = DSDIFF_TYPE;
        }
        else if (ext == ".dsf") {
            mediaType = DSF_TYPE;
        }
        if (mediaType == UNK_TYPE) {
            cerr << "PANIC: exception_io_unsupported_format\n";
            exit(1);
        }
	return mediaType;
}
//From a filename, produce a media object
sacd_media_t* getMedium(string inFile) {
	sacd_media_t* sacdMedium;
	sacdMedium = new sacd_media_t();
	if (!sacdMedium) {
            cerr << "PANIC: exception_overflow\n";
            exit(1);
        }
	return sacdMedium;
}

//From a filename and media object, produce a reader object
sacd_reader_t* getReader(string inFile,sacd_media_t* medium) {
	sacd_reader_t* sacdReader;
	media_type_t type = getMediaType(inFile);

        switch (type) {
            case ISO_TYPE:
                sacdReader = new sacd_disc_t;
                if (!sacdReader) {
                    cerr << "PANIC: exception_overflow\n";
                    exit(1);
                }
                break;
            case DSDIFF_TYPE:
                sacdReader = new sacd_dsdiff_t;
                if (!sacdReader) {
                    cerr << "PANIC: exception_overflow\n";
                    exit(1);
                }
                break;
	    case DSF_TYPE:
                sacdReader = new sacd_dsf_t;
                if (!sacdReader) {
                    cerr << "PANIC: exception_overflow\n";
                    exit(1);
                }
                break;
            default:
                cerr << "PANIC: exception_io_data\n";
                exit(1);
                break;
        }

        if (!medium->open(inFile.c_str())) {
            cerr << "PANIC: exception_io_data\n";
            exit(1);
        }

        if (sacdReader->open(medium) == 0) {
            cerr << "PANIC: Failed to parse SACD media\n";
            exit(1);
        }

	return sacdReader;
}

//For rendering most of the information out.
void getTrack(sacd_reader_t *reader, unsigned short t, area_id_e section, bool JSON) {
	TrackDetails	details;
	reader->getTrackDetails(t, section, &details);

	if (JSON) {
		cout << "{"
			"\"Track\": " << t+1 << ","
			"\"Artist\": \"" << details.strArtist.data() << "\","
			"\"Title\": \"" << details.strTitle.data() << "\","
			"\"Channels\": " << details.nChannels
			<< "}";
	} else {
		cout << "\t  Track:    " << t + 1 << "\n"
			"\t  Artist:   " << details.strArtist.data() << "\n"
			"\t  Title:    " << details.strTitle.data() << "\n"
			"\t  Channels: " << details.nChannels << "\n\n";
	}
}

int getSection(sacd_reader_t *reader, area_id_e section, bool JSON) {
	unsigned short	numTracks = reader->get_track_count(section);
	unsigned short	t;
	TrackDetails	details;
	string		secName;

	if (section == AREA_TWOCH)	secName = "Stereo";
	else if (section == AREA_MULCH)	secName = "Multichannel";

	if (JSON)	cout << "\"" << secName << "\": [";
	else		cout << "\t" << secName << " section:\t" << numTracks <<" tracks.\n";

	if (numTracks > 0) {
		for (t = 0;t<numTracks;t++) {
			getTrack(reader,t,section,JSON);
			if (JSON && t+1 < numTracks) cout << ", ";
		}
		
	} else if (!JSON) cout << "\tSection empty.\n";
	if (JSON)	cout << "]";
	return numTracks;
}

int main(int argc, char *argv[]) {
	sacd_media_t*	sacdMedium;
	sacd_reader_t*	sacdReader;
	int		opt = 0;
	unsigned short	findex;
	unsigned short	filecount;
	ifstream	inFile;
	ofstream	outFile;
	streambuf*	console = cout.rdbuf();

	//Parameters set by command-line
	string	outFn		= "";
	bool	JSON		= false;
	bool	sectStereo	= true;
	bool	sectMulti	= true;
	unsigned short	track	= 0;

	while ((opt = getopt_long(argc,argv,"o:js:t:h",stGetOptions,NULL)) >= 0) {
		switch (opt) {
			case 'o':
				outFn = optarg;
				break;
			case 'j':
				JSON=true;
				break;
			case 's':
				if (tolower(optarg[0]) == 's') sectMulti = false;
				else if (tolower(optarg[0]) == 'm') sectStereo = false;
				else {
					cerr << "Bad section name: " << optarg << "\n";
					exit(1);
				}
				break;
			case 't':
				track = (unsigned short)stoi(optarg);
				if(!track) {
					cerr << "Tracks start at zero.\n";
					exit(1);
				}
				break;
			case 'h':
				cerr << stHelpHeader << argv[0] << stHelpMessage;
				exit(1);
				break;	//Not really
			case '?':
				cerr << stHelpHeader << argv[0] << stHelpMessage;
				exit(1);
				break;	//Not really
			default:
				cerr << stHelpHeader << argv[0] << stHelpMessage;
				exit(1);
				break; // but again, no.
		}
	}

	//Now everything ought to be files
	filecount=argc-optind;
	if (filecount <1) {
		cerr << "No files given on the command-line.\n";
		exit(1);
	}
	//Make sure they can all be opened for reading
	for (findex = optind;findex < argc;findex++) {
		inFile = ifstream(argv[findex]);
		if (!inFile.is_open()) {
			cerr << "Can't open file: " << argv[findex] << "\n";
			exit(1);
		}
		inFile.close();	
	}

	//In case we have an actual output file
	if (outFn != "") {
		//First, see if it exists and bail out, if so.
		inFile = ifstream(outFn);
		if (inFile.is_open()) {
			inFile.close();
			cerr << "Output file already exists: " << outFn << "\nWon't overwrite.\n\n";
			exit(1);
		}

		// Now try to open it for writing.
		outFile = ofstream(outFn);
		if (!outFile.is_open()) {
			cerr << "Can't open output file for writing: " << outFn << "\n";
		}
		//We'll just redirect stdout here and use the output stream.
		cout.rdbuf(outFile.rdbuf());
	}

	for (unsigned short findex = optind;findex < argc;findex++) {
		sacdMedium = getMedium(argv[findex]);
		sacdReader = getReader(argv[findex],sacdMedium);

		//In case we just want a single track
		if (track) {
			if (sectStereo && sectMulti) {
				cerr << "Please also specify a section.\n";
				exit(1);
			}
			if (filecount > 1) {
				cerr << "For a single track, please only specify one file.\n";
				exit(1);
			}

			if (sectStereo) {
				if (sacdReader->get_track_count(AREA_TWOCH) <= track) {
					cerr << "Track " << track << " does not exist.\n";
					exit(1);
				}	
				getTrack(sacdReader, track-1, AREA_TWOCH, JSON);
			}
			if (sectMulti) {
				if (sacdReader->get_track_count(AREA_MULCH) <= track) {
					cerr << "Track " << track << " does not exist.\n";
					exit(1);
				}
				getTrack(sacdReader, track-1, AREA_MULCH, JSON);
			}
			cout << "\n";
			break;
		}

		if (JSON) {
			if (filecount > 1) cout <<"[ ";
			cout << "{\"File\": \"" << argv[findex] << "\", ";
		} else	cout << "File: " << argv[findex] << ":\n";

		if (sectStereo) {
			getSection(sacdReader, AREA_TWOCH, JSON);
		}	
		if (sectMulti) {
			if (JSON && sectStereo) cout << ", ";
			getSection(sacdReader, AREA_MULCH, JSON);
		}
		
		if (JSON) {
			cout << "}";
		       if (filecount > 1) {
			       if (findex + 1 == argc) {
					cout << ", ";
			       } else {
			       		cout << " ]";
			       }
		       }
		} 
		cout << "\n";

                inFile.close();
		delete sacdMedium;
		delete sacdReader;
		inFile.close();
        }

	cout.rdbuf(console);
        outFile.close();

	exit(0);


}


