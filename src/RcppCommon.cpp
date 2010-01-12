// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppCommon.cpp: R/C++ interface class library -- common functions
//
// Copyright (C) 2005 - 2006 Dominick Samperi
// Copyright (C) 2008 - 2009 Dirk Eddelbuettel
//
// This file is part of Rcpp.
//
// Rcpp is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

#include <Rcpp.h>
#include <cstring>

// Paul Roebuck has observed that the memory used by an exception message
// is not reclaimed if error() is called inside of a catch block (due to
// a setjmp() call), and he suggested the following work-around.
char *copyMessageToR(const char* const mesg) {
    char* Rmesg;
    const char* prefix = "Exception: ";
    void* Rheap = R_alloc(strlen(prefix)+strlen(mesg)+1,sizeof(char));
    Rmesg = static_cast<char*>(Rheap);
    strcpy(Rmesg, prefix);
    strcat(Rmesg, mesg);
    return Rmesg;
}

inline void logTxtFunction(const char* file, const int line, const char* expression) {
    Rprintf("%s:%d %s\n", file, line, expression);
}

SEXP test_variadic() {
	SEXP res = PROTECT( Rf_allocVector(INTSXP, 5) ) ; 
#ifdef HAS_VARIADIC_TEMPLATES
	INTEGER(res)[0] = variadic_length() ; 
	INTEGER(res)[1] = variadic_length(1) ;
	INTEGER(res)[2] = variadic_length(1, 3.3) ;
	INTEGER(res)[3] = variadic_length(1, "foo", 'f') ;
	INTEGER(res)[4] = variadic_length(1, 2, 2.3f, "foo", std::string("foobar") ) ;
#else
	INTEGER(res)[0] = 0 ; 
	INTEGER(res)[1] = 1 ;
	INTEGER(res)[2] = 2 ;
	INTEGER(res)[3] = 3 ;
	INTEGER(res)[4] = 4 ;
#endif
	UNPROTECT(1) ;
	return res;
}

SEXP canUseCXX0X(){
#ifdef HAS_VARIADIC_TEMPLATES
	return Rf_ScalarLogical( TRUE ) ;
#else
	return Rf_ScalarLogical( FALSE ) ;
#endif
}

SEXP capabilities(){
	SEXP cap = PROTECT( Rf_allocVector( LGLSXP, 3) ) ;
	SEXP names = PROTECT( Rf_allocVector( STRSXP, 3 ) ) ;
#ifdef HAS_VARIADIC_TEMPLATES
	LOGICAL(cap)[0] = TRUE ;
#else
	LOGICAL(cap)[0] = FALSE ;
#endif
#ifdef HAS_INIT_LISTS
	LOGICAL(cap)[1] = TRUE ;
#else
	LOGICAL(cap)[1] = FALSE ;
#endif
#ifdef __GNUC__
	LOGICAL(cap)[2] = TRUE ;
#else
	/* just because I don't know */
	LOGICAL(cap)[2] = FALSE ;
#endif

	SET_STRING_ELT(names, 0, Rf_mkChar("variadic templates") ) ;
	SET_STRING_ELT(names, 1, Rf_mkChar("initializer lists") ) ;
	SET_STRING_ELT(names, 2, Rf_mkChar("exception handling") ) ;
	Rf_setAttrib( cap, R_NamesSymbol, names ) ;
	UNPROTECT(2) ;
	return cap ;
}


/* this is mainly here so that variadic template errors show up 
   at compile time */
SEXP test_named(){
#ifdef HAS_VARIADIC_TEMPLATES
	return Rcpp::Language( "foobar", Rcpp::Named("foo", 2 ), 2, Rcpp::Named("bar", 10) ) ;
#else
	return R_NilValue ;
#endif
}

const char * const sexp_to_name(int sexp_type) {
    switch (sexp_type) {
    case NILSXP:	return "NILSXP";
    case SYMSXP:	return "SYMSXP";
    case LISTSXP:	return "LISTSXP";
    case CLOSXP:	return "CLOSXP";
    case ENVSXP:	return "ENVSXP";
    case PROMSXP:	return "PROMSXP";
    case LANGSXP:	return "LANGSXP";
    case SPECIALSXP:	return "SPECIALSXP";
    case BUILTINSXP:	return "BUILTINSXP";
    case CHARSXP:	return "CHARSXP";
    case LGLSXP:	return "LGLSXP";
    case INTSXP:	return "INTSXP";
    case REALSXP:	return "REALSXP";
    case CPLXSXP:	return "CPLXSXP";
    case STRSXP:	return "STRSXP";
    case DOTSXP:	return "DOTSXP";
    case ANYSXP:	return "ANYSXP";
    case VECSXP:	return "VECSXP";
    case EXPRSXP:	return "EXPRSXP";
    case BCODESXP:	return "BCODESXP";
    case EXTPTRSXP:	return "EXTPTRSXP";
    case WEAKREFSXP:	return "WEAKREFSXP";
    case S4SXP:		return "S4SXP";
    default:
	return "<unknown>";
    }
}


