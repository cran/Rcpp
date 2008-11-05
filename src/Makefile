# -*- tab-width: 8 -*- 
#
# Makefile for Rcpp 
#
# Copyright (C) 2008    Dirk Eddelbuettel <edd@debian.org>

RCPPFLAGS	= `${R_HOME}/bin/R CMD config --cppflags`
RLDFLAGS	= `${R_HOME}/bin/R CMD config --ldflags`

## the 'package' library contains both Rcpp.{cpp,h} and the RcppExample used to demonstrate the package
## it is loaded via library(Rcpp) but is not used for compiling / linking against
PKGLIB 		= Rcpp$(DYLIB_EXT)
PKGOBJ 		= RcppExample.o Rcpp.o
PKGLDFLAGS 	= -s $(RLDFLAGS) 

## the 'user' library contains only Rcpp.{cpp,h} and is what users need for their projects
## we place it inside the inst/ directory so that it gets installed 
USERDIR		= ../inst/lib
USERLIB		= $(USERDIR)/libRcpp$(DYLIB_EXT)
USEROBJ		= Rcpp.o

## more correct version of shared library build
#		= gcc -shared -Wl,-soname,librcpp.so.5 \
#                                -o ../librcpp.so.5.0 Rcpp.o
## where you'd still need the softlink from librcpp.so.5.0 to librcpp.so

CXXFLAGS 	= -I. $(RCPPFLAGS) -Wall -O2 
RM 		= rm -f

.PHONY: 	all clean

all: 		$(USERLIB) $(PKGLIB)

clean:
		${RM} $(PKGOBJ) $(PKGLIB) $(USERLIB) $(USEROBJ)

$(PKGLIB): 	$(PKGOBJ)
		$(CXX) -shared -o $(PKGLIB) $(PKGOBJ) $(PKGLDFLAGS)

$(USERLIB):	$(USEROBJ)
		$(CXX) -shared -o $(USERLIB) $(USEROBJ) 
		@cp -vax Rcpp.h $(USERDIR)

#RLIBnn		= `${R_HOME}/bin/R CMD config`
#RLIBDIR	= ${R_HOME}/$(RLIBnn)
#		@echo "****"
#		@echo "**** You may need to execute the following command as root"
#		@echo "**** so that the libRcpp library will get found by R at runtime"
#		@echo "     cp -vax $(USERLIB) $(RLIBDIR)"
#		@echo "**** with a similar copy of Rcpp.h to R's include directory"
#		@echo "****"

