PNAME=sacdinfo

SACDBUILD=../..
SACDLIB=sacd

SLBUILDVER=19
SLBUILDNAME=lib${SACDLIB}.so.${SLBUILDVER}

SLSTATICOBJS=sacd_media.o scarletbook.o sacd_disc.o sacd_dsdiff.o sacd_dsf.o
SLSTATLINK=$(foreach obj,$(SLSTATICOBJS),$(SACDBUILD)/lib$(SACDLIB)/$(obj))

ARCH = $(shell getconf LONG_BIT)

CXX = g++

CXXFLAGS_32 = -msse2
CXXFLAGS_64 =
CXXFLAGS = $(CXXFLAGS_$(ARCH)) -std=c++11 -Wall -O3

CPPFLAGS += -I${SACDBUILD}

PREFIX := /usr

.PHONY: all clean install

all: clean $(PNAME)

main: main.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c main.cpp -o main.o

$(PNAME): main $(SLSTATLINK)
	$(CXX) $(CXXFLAGS) -o ${PNAME} main.o $(LDFLAGS) $(SLSTATLINK)	
	
shared: main ${SACDBUILD}/${SLBUILDNAME}
	$(CXX) $(CXXFLAGS) -o ${PNAME} main.o $(LDFLAGS) ${SACDBUILD}/${SLBUILDNAME}

clean:
	rm -f $(PNAME) *.o

install: ${PNAME}

	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 0755 $(PNAME) $(DESTDIR)$(PREFIX)/bin
	install -d $(DESTDIR)$(PREFIX)/share/man/man1
	install -m 0644 $(PNAME).1 $(DESTDIR)$(PREFIX)/share/man/man1

