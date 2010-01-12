// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppCommon.h: Rcpp R/C++ interface class library -- common include and defines statements
//
// Copyright (C) 2005 - 2006 Dominick Samperi
// Copyright (C) 2008 - 2009 Dirk Eddelbuettel
// Copyright (C)        2009 Romain Francois
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

#ifndef RcppCommon_h
#define RcppCommon_h

#ifdef __GNUC__
	#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
	#if GCC_VERSION >= 40300
		#define HAS_VARIADIC_TEMPLATES
	#endif
	#if GCC_VERSION >= 40400
		#define HAS_INIT_LISTS
	#endif
#endif

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <stdexcept>
#include <vector>

// include R headers, but set R_NO_REMAP and access everything via Rf_ prefixes
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Callbacks.h>
#include <R_ext/Complex.h>
#include <Rversion.h>
#define RCPP_GET_NAMES(x)	Rf_getAttrib(x, R_NamesSymbol)

// #ifdef BUILDING_DLL
// #define RcppExport extern "C" __declspec(dllexport)
// #else
#define RcppExport extern "C"
// #endif

char *copyMessageToR(const char* const mesg);

// simple logging help
inline void logTxtFunction(const char* file, const int line, const char* expression); 

#ifndef logTxt
//#define logTxt(x) logTxtFunction(__FILE__, __LINE__, x);
#define logTxt(x) 
#endif

/* in exceptions.cpp */
#ifdef __GNUC__
void forward_uncaught_exceptions_to_r() ;
#endif 

RcppExport SEXP initUncaughtExceptionHandler() ; 

/* just testing variadic templates */
#ifdef HAS_VARIADIC_TEMPLATES
template<typename... Args>
int variadic_length( const Args&... args) { return sizeof...(Args) ; }
#endif

RcppExport SEXP test_variadic() ; 
RcppExport SEXP canUseCXX0X() ;
RcppExport SEXP test_named() ;
RcppExport SEXP capabilities() ;

const char * const sexp_to_name(int sexp_type); 

#endif
