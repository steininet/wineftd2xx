#Makefile for wineftd2xx<->ftd2xx shim dll
#Revised:  9/30/14 brent@mbari.org
#WARNING:  omitting frame pointer causes crashes
#Info:     tackleSIGSEGV11 steini 6.4.19
CFLAGS = -g -O0 -Wall 
LIBS=libxftd2xx.a -ldl -lrt -lpthread

RELEASE=wineftd2xx1.1.2

ARCH ?= $(shell uname -m)

WINEDLLPATH := $(shell ./winedllpath $(ARCH))
ifeq ("$(WINEDLLPATH)", "")
$(error Can't guess WINEDLLPATH -- \
  specify it with make WINEDLLPATH={path_to_dll.so directory})
endif
$(info WINEDLLPATH=$(WINEDLLPATH))

#path to FTDI's linux libftd2xx1.1.12 top-level directory
LIBFTD = libftd2xx-i386-1.4.8
IDIR = $(LIBFTD)
TARBALL = $(LIBFTD).gz

sixty4 := $(findstring 64-bit, $(shell file $(WINEDLLPATH)/version.dll.so))

ifeq (,$(ARCH))
ifneq (,$(sixty4))
ARCH = x86_64
else
ARCH = i386
endif
endif
ARCHIVE = $(LIBFTD)/build/libftd2xx.a
$(info Link with $(ARCHIVE))

ifeq (i386,$(ARCH))
CFLAGS += -m32
endif

all: libftd2xx.def ftd2xx.dll.so

$(TARBALL):
	wget https://www.ftdichip.com/Drivers/D2XX/Linux/libftd2xx-i386-1.4.8.gz
	touch -t 1201010000 $(TARBALL)

$(ARCHIVE) $(IDIR)/ftd2xx.h:  $(TARBALL)
	tar xzf $(TARBALL)
	rm -rf $(LIBFTD)
	mv release $(LIBFTD)

libxftd2xx.a:  $(ARCHIVE) xFTsyms.objcopy
	objcopy --redefine-syms=xFTsyms.objcopy $(ARCHIVE) libxftd2xx.a

xftd2xx.h:  $(IDIR)/ftd2xx.h Makefile
	sed "s/WINAPI FT_/WINAPI xFT_/g" $(IDIR)/ftd2xx.h >$@

ftd2xx.o: ftd2xx.c xftd2xx.h WinTypes.h
	winegcc -D_REENTRANT -D__WINESRC__ -c $(CFLAGS) \
          -I$(IDIR) -fno-omit-frame-pointer -o $@ ftd2xx.c

ftd2xx.dll.so: ftd2xx.o ftd2xx.spec libxftd2xx.a
	winegcc $(CFLAGS) -lntdll -lkernel32 \
          -o ftd2xx.dll ftd2xx.o libxftd2xx.a -shared ftd2xx.spec $(LIBS)
#          -o ftd2xx.dll ftd2xx.o libxftd2xx.a -shared ftd2xx.spec $(LIBS)
#	winegcc $(CFLAGS) -mwindows -lntdll -lkernel32 \

libftd2xx.def: ftd2xx.spec ftd2xx.dll.so
	winebuild -w --def -o $@ --export ftd2xx.spec

install:        ftd2xx.dll.so libftd2xx.def
	cp ftd2xx.dll.so libftd2xx.def  $(WINEDLLPATH)

uninstall:
	rm -f $(WINEDLLPATH)/ftd2xx.dll.so $(WINEDLLPATH)/libftd2xx.def

clean:
	rm -f *.o *xftd2xx.* *.so *.def

distclean:  clean
	rm -rf $(LIBFTD) $(RELEASE)
	rm -f $(TARBALL) $(RELEASE).tar.gz

release:
	rm -rf $(RELEASE)
	mkdir $(RELEASE)
	cp -a etc *.c *.h *.spec *.objcopy Makefile winedllpath README* \
	  $(RELEASE)
	tar zvcf $(RELEASE).tar.gz $(RELEASE)

