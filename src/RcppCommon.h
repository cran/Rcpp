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
	#ifdef __GXX_EXPERIMENTAL_CXX0X__
		#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
		#if GCC_VERSION >= 40300
			#define HAS_VARIADIC_TEMPLATES
		#endif
		#if GCC_VERSION >= 40400
			#define HAS_INIT_LISTS
		#endif
	#endif
#endif

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>
#include <deque>
#include <functional>
#include <numeric>
#include <algorithm>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#endif

// include R headers, but set R_NO_REMAP and access everything via Rf_ prefixes
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Callbacks.h>
#include <R_ext/Complex.h>
#include <R_ext/Parse.h>
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
void forward_uncaught_exceptions_to_r() ;
RcppExport SEXP initUncaughtExceptionHandler() ; 

/* just testing variadic templates */
#ifdef HAS_VARIADIC_TEMPLATES
template<typename... Args>
int variadic_length( const Args&... args) { return sizeof...(Args) ; }
#endif

#ifdef HAS_VARIADIC_TEMPLATES
RcppExport inline SEXP canUseCXX0X(){ return Rf_ScalarLogical( TRUE ); }
#else
RcppExport inline SEXP canUseCXX0X(){ return Rf_ScalarLogical( FALSE ); }
#endif

RcppExport SEXP test_named() ;
RcppExport SEXP capabilities() ;

const char * sexp_to_name(int sexp_type); 

RcppExport SEXP initRcpp() ;

namespace Rcpp{
/* internal namespace for things not intended to be used by the 
   user */
namespace internal{	
	
	/* defined in Evaluator.cpp */
	SEXP convert_using_rfunction(SEXP x, const char* const fun) ;
	
	SEXP try_catch( SEXP expr, SEXP env ) ;
	SEXP try_catch( SEXP expr ) ;
	
} // namespace internal 

/** 
 * Generic converted from SEXP to the typename. T can be any type that 
 * has a constructor taking a SEXP, which is the case for all our 
 * RObject and derived classes. 
 *
 * If it is not possible to add the SEXP constructor, e.g you don't control
 * the type, you can overload the as template to perform the 
 * requested conversion
 *
 * This is used for example in Environment, so that for example the code
 * below will work as long as there is a way to as<> the Foo type
 *
 * Environment x = ... ; // some environment
 * Foo y = x["bla"] ;    // if as<Foo> makes sense then this works !!
 */
template <typename T> T as( SEXP m_sexp) {
	T t(m_sexp);
	return t ;
}
template<> bool 			as<bool>(SEXP m_sexp) ;
template<> double                   	as<double>(SEXP m_sexp) ;
template<> int                      	as<int>(SEXP m_sexp) ;
template<> Rbyte                    	as<Rbyte>(SEXP m_sexp) ;
template<> std::string              	as<std::string>(SEXP m_sexp) ;
template<> std::vector<int>         	as< std::vector<int> >(SEXP m_sexp) ;
template<> std::vector<double>      	as< std::vector<double> >(SEXP m_sexp) ;
template<> std::vector<std::string> 	as< std::vector<std::string> >(SEXP m_sexp) ;
template<> std::vector<Rbyte>       	as< std::vector<Rbyte> >(SEXP m_sexp) ;
template<> std::vector<bool>        	as< std::vector<bool> >(SEXP m_sexp) ;


/* FIXME: turn the functions below into a template */

/* these do not take care of coercion*/
inline bool Rboolean_to_bool( int x){ return x == TRUE ; }
inline bool int_to_bool(int x){ return x != 0 ; }
inline bool double_to_bool(double x){ return x != 0.0 ; }
inline bool Rbyte_to_bool(Rbyte x){ return x != static_cast<Rbyte>(0) ; }

/* these take care of coercion */
inline int bool_to_Rboolean(bool x){ return x ? TRUE : FALSE ; }

inline int Rboolean_to_int(int x){ return (x==NA_LOGICAL) ? NA_INTEGER : x ; }
inline int double_to_int(double x){ 
	if (ISNAN(x)) return NA_INTEGER;
	else if (x > INT_MAX || x <= INT_MIN ) {
		return NA_INTEGER;
	}
	return static_cast<int>(x);
}
inline int Rbyte_to_int(Rbyte x){ return static_cast<int>(x); }

inline Rbyte Rboolean_to_Rbyte(int x){ return x == TRUE ? static_cast<Rbyte>(1) : static_cast<Rbyte>(0) ;}
inline Rbyte double_to_Rbyte(double x){ 
	if( x == NA_REAL) return (Rbyte)0 ; 
	int y = static_cast<int>(x) ;
	return (y < 0 || y > 255) ? (Rbyte)0 : (Rbyte)y ;
} 
inline Rbyte int_to_Rbyte(int x){
	return (x < 0 || x > 255) ? static_cast<Rbyte>(0) : static_cast<Rbyte>(x) ;
}

inline double Rbyte_to_double(Rbyte x){
	return static_cast<double>(x) ;
}
inline double int_to_double(int x){
	return x == NA_INTEGER ? NA_REAL : static_cast<double>(x) ;
}
inline double Rboolean_to_double(int x){
	return x == NA_LOGICAL ? NA_REAL : static_cast<double>(x) ;
}

inline int int_to_RBoolean(int x){ return ( x == NA_INTEGER ) ? NA_LOGICAL : (x!=0); }

} // namespace Rcpp

#include <Rcpp/internal/wrap.h>

#include <Rcpp/RObject.h>

#endif
