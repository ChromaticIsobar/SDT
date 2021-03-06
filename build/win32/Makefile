SHELL=/bin/bash

CC=i686-w64-mingw32-gcc
CFLAGS=-Wall -Wno-unknown-pragmas -Werror -O3
LDFLAGS=-shared -static-libgcc
SRCDIR=../../src
THIRDPDIR=../../3rdparty
# DSTDIR=../../SoundDesignToolkit

SDTDIR=$(SRCDIR)/SDT
SDTSRCS=$(wildcard $(SDTDIR)/*.c)
SDTOBJS=$(patsubst %.c,%.o,$(SDTSRCS))

MAXDIR=$(SRCDIR)/Max7
MAXSRCS=$(wildcard $(MAXDIR)/*.c)
MAXOBJS=$(patsubst %.c,%.mxe,$(MAXSRCS))

MAXSDKDIR=$(THIRDPDIR)/Max7SDK
MAXSDKVERSION=7.3.3

MAXPACKDIR=../../MaxPackage

PDDIR=$(SRCDIR)/Pd
PDSRCS=$(wildcard $(PDDIR)/*.c)
PDOBJS=$(patsubst %.c,%.o,$(PDSRCS))

PDSDKDIR=$(THIRDPDIR)/Pd
PDSDKVERSION=0.41.4

JSONDIRB=$(THIRDPDIR)/json-builder
JSONSRCB=$(wildcard $(JSONDIRB)/*.c)
JSONOBJB=$(patsubst %.c,%.o,$(JSONSRCB))

JSONDIRP=$(THIRDPDIR)/json-parser
JSONSRCP=$(wildcard $(JSONDIRP)/*.c)
JSONOBJP=$(patsubst %.c,%.o,$(JSONSRCP))

INCLUDE_JSON=-I$(JSONDIRP) -I$(JSONDIRB)
JSONOBJS=$(JSONOBJP) $(JSONOBJB)

all: core max pd

pd: $(SDTOBJS) $(PDOBJS) $(JSONOBJS)
	$(CC) $(LDFLAGS) -L$(PDSDKDIR) $(JSONOBJS) $(SDTOBJS) $(PDOBJS) -o $(PDDIR)/SDT.dll -lpd

$(PDDIR)/%.o: $(PDDIR)/%.c
	$(CC) $(CFLAGS) -I$(SRCDIR) -I$(PDSDKDIR) $(INCLUDE_JSON) -c $< -o $@

max: core fileusage $(MAXOBJS)

$(MAXDIR)/%.mxe: EXTNAME=$(patsubst $(MAXDIR)/%.mxe,%,$@)
$(MAXDIR)/%.mxe: $(MAXDIR)/%.o
	cp Info.def $(MAXDIR)/$(EXTNAME).def
	sed -i $(MAXDIR)/$(EXTNAME).def -e s/\$${PRODUCT_NAME}/$(EXTNAME)/g
	$(CC) $(LDFLAGS) -L$(MAXSDKDIR)/max-includes -L$(MAXSDKDIR)/msp-includes -L$(MAXSDKDIR)/jit-includes -L$(SDTDIR) \
	$< $(MAXDIR)/$(EXTNAME).def -o $@ $(MAXDIR)/SDT_fileusage.o -lMaxAPI -lMaxAudio -lSDT

$(MAXDIR)/%.o: $(MAXDIR)/%.c $(MAXDIR)/SDT_fileusage/SDT_fileusage.c
	$(CC) $(CFLAGS) -DDENORM_WANT_FIX=1 -DNO_TRANSLATION_SUPPORT=1 -DC74_NO_DEPRECATION \
	$(INCLUDE_JSON) \
	-DWIN_VERSION -DWIN_EXT_VERSION -fvisibility=hidden \
	-I$(MAXSDKDIR)/max-includes -I$(MAXSDKDIR)/msp-includes -I$(MAXSDKDIR)/jit-includes \
	-I$(SRCDIR) -c $< -o $@

fileusage:
	$(CC) $(CFLAGS) -DDENORM_WANT_FIX=1 -DNO_TRANSLATION_SUPPORT=1 -DC74_NO_DEPRECATION \
	-DWIN_VERSION -DWIN_EXT_VERSION -fvisibility=hidden \
	-I$(MAXSDKDIR)/max-includes -I$(MAXSDKDIR)/msp-includes -I$(MAXSDKDIR)/jit-includes \
	-I$(SRCDIR) -c $(MAXDIR)/SDT_fileusage/SDT_fileusage.c -o $(MAXDIR)/SDT_fileusage.o

core: $(SDTOBJS) json
	$(CC) $(LDFLAGS) $(SDTOBJS) $(JSONOBJS) -o $(SDTDIR)/libSDT.dll

$(SDTDIR)/%.o: $(SDTDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE_JSON) -c $< -o $@

json: $(JSONOBJP) $(JSONOBJB)

$(JSONDIRP)/%.o: $(JSONDIRP)/%.c
	$(CC) $(CFLAGS) $(INCLUDE_JSON) -c $< -o $@

$(JSONDIRB)/%.o: $(JSONDIRB)/%.c
	$(CC) $(CFLAGS) $(INCLUDE_JSON) -c $< -o $@

install_max:
	@if [ -z "$(DSTDIR)" ]; then \
		echo "Usage: make install_max DSTDIR=<installation_path>"; \
	elif [ ! -d "$(DSTDIR)" ]; then \
		echo "Error: installation path does not exist or is not a directory"; \
	else \
		mkdir -p "${DSTDIR}/Sound Design Toolkit/"{externals,support} && \
		cp -a $(MAXPACKDIR)/* "${DSTDIR}/Sound Design Toolkit/" && \
		cp -a $(SDTDIR)/libSDT.dll "${DSTDIR}/Sound Design Toolkit/support" && \
		cp -a $(MAXDIR)/*.mxe "${DSTDIR}/Sound Design Toolkit/externals" && \
		echo "Sound Design Toolkit package for Max installed in '${DSTDIR}/Sound Design Toolkit'"; \
	fi

uninstall_max:
	@if [ -z "$(DSTDIR)" ]; then \
		echo "Usage: make uninstall_max DSTDIR=<installation_path>"; \
	elif [ ! -d "$(DSTDIR)" ]; then \
		echo "Error: installation path does not exist or is not a directory"; \
	else \
		rm -rf "${DSTDIR}/Sound Design Toolkit/" && \
		echo "Sound Design Toolkit package for Max removed from '${DSTDIR}/Sound Design Toolkit'"; \
	fi

install_pd:
	@if [ -z "$(DSTDIR)" ]; then \
		echo "Usage: make install_pd DSTDIR=<installation_path>"; \
	elif [ ! -d "$(DSTDIR)" ];  then \
		echo "Error: installation path does not exist or is not a directory"; \
	else \
		mkdir -p "$(DSTDIR)"/SDT && \
		cp -a ../../Pd/* "$(DSTDIR)"/SDT && \
		cp -a $(PDDIR)/SDT.dll "$(DSTDIR)"/SDT && \
		echo "Sound Design Toolkit for Pure Data installed in '$(DSTDIR)/SDT'"; \
	fi

uninstall_pd:
	@if [ -z "$(DSTDIR)" ]; then \
		echo "Usage: make uninstall_pd DSTDIR=<installation_path>"; \
	elif [ ! -d "$(DSTDIR)" ];  then \
		echo "Error: installation path does not exist or is not a directory"; \
	else \
		rm -rf "$(DSTDIR)"/SDT && \
		echo "Sound Design Toolkit for Pure Data removed from '$(DSTDIR)/SDT'"; \
	fi

install_core:
	@if [ -z "$(DSTDIR)" ]; then \
		echo "Usage: make install_core DSTDIR=<installation_path>"; \
	elif [ ! -d "$(DSTDIR)" ]; then \
		echo "Error: installation path does not exist or is not a directory"; \
	else \
		mkdir -p "$(DSTDIR)"/include/SDT && \
		mkdir -p "$(DSTDIR)"/lib && \
		cp -a $(SDTDIR)/libSDT.dll "$(DSTDIR)"/lib && \
		cp -a $(SDTDIR)/*.h $(DSTDIR)/include/SDT && \
		cp -a $(JSONDIRP)/*.h $(DSTDIR)/include/SDT && \
		cp -a $(JSONDIRB)/*.h $(DSTDIR)/include/SDT && \
		echo "Sound Design Toolkit 'Core Library' installed in '$(DSTDIR)'"; \
	fi

uninstall_core:
	@if [ -z "$(DSTDIR)" ]; then \
		echo "Usage: make uninstall_core DSTDIR=<installation_path>"; \
	elif [ ! -d "$(DSTDIR)" ]; then \
		echo "Error: installation path does not exist or is not a directory"; \
	else \
		rm -rf "$(DSTDIR)"/include/SDT && \
		rm -f "$(DSTDIR)"/lib/libSDT.dll && \
		echo "Sound Design Toolkit 'Core Library' removed from '$(DSTDIR)'"; \
	fi

clean:
	rm -rf "$(SDTDIR)/"libSDT.dll
	rm -rf "$(SDTDIR)/"*.o
	rm -rf "$(MAXDIR)/"*.mxe
	rm -rf "$(MAXDIR)/"*.def
	rm -rf "$(MAXDIR)/"*.o
	rm -rf "$(PDDIR)/"*.dll
	rm -rf "$(PDDIR)/"*.o
	rm -rf "$(JSONDIRP)/"*.o
	rm -rf "$(JSONDIRB)/"*.o
