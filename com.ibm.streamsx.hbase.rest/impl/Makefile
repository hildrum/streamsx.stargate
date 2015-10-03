# Copyright (C) 2015, International Business Machines Corporation  
# All Rights Reserved

ifeq ($(CXX),g++)
	  PLATFORM := $(shell uname -i)
	    ifeq ($(PLATFORM),ppc64)
		    CXX=/opt/at5.0/bin/g++
			  endif
		  endif
		  SPL_PKGCFG=$(STREAMS_INSTALL)/bin/dst-pe-pkg-config.sh
		  SPL_PKG=dst-spl-pe-install
		  SPL_COMPILE_OPTIONS = `$(SPL_PKGCFG) --cflags $(SPL_PKG)`
.PHONY: clean distclean

all: lib/libstargate.so

lib:
		mkdir lib

bin:
		mkdir bin

lib/libstargate.so: bin/base64.o lib
		gcc --shared -o lib/libstargate.so bin/base64.o

bin/base64.o: src/base64.cpp bin
	@$(CXX) -O3 -Wall -fPIC -I include $(SPL_COMPILE_OPTIONS) -c $< -o $@

clean:
		rm lib/*.so
			rm bin/*.o

distclean: clean
