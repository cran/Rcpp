// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// LogicalVector.h: Rcpp R/C++ interface class library -- logical vectors
//
// Copyright (C) 2010	Dirk Eddelbuettel and Romain Francois
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

#ifndef Rcpp_LogicalVector_h
#define Rcpp_LogicalVector_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/VectorBase.h>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#include <algorithm>
#endif

namespace Rcpp{

class LogicalVector : public VectorBase {     
public:

	LogicalVector(SEXP x) throw(not_compatible);
	LogicalVector( int size) ;

#ifdef HAS_INIT_LISTS	
	LogicalVector( std::initializer_list<Rboolean> list ) : VectorBase(), start(0) {
		simple_fill( list.begin(), list.end() ) ;
	}
	LogicalVector( std::initializer_list<int> list ) : VectorBase(), start(0) {
		coerce_and_fill( list.begin(), list.end() ) ;
	}
	LogicalVector( std::initializer_list<bool> list ) : VectorBase(), start(0) {
		coerce_and_fill( list.begin(), list.end() ) ;
	}
#endif

	typedef int* iterator ;

	inline int& operator[]( int i ) const { return start[i] ;}
	inline int* begin() const { return start ; }
	inline int* end() const { return start + LENGTH(m_sexp); }

private:
	int* start ;
	virtual void update(){ start=LOGICAL(m_sexp); }
	
	// called when there is no need for coercion
	template <typename InputIterator>
	void simple_fill( InputIterator first, InputIterator last){
		size_t size = std::distance( first, last ) ;
		SEXP x = PROTECT( Rf_allocVector( LGLSXP, size ) ) ;
		std::copy( first, last, LOGICAL(x) ); 
		setSEXP(x) ;
		UNPROTECT( 1 ); /* x */
	}
	
	template <typename InputIterator>
	void coerce_and_fill( InputIterator first, InputIterator last){
		size_t size = std::distance( first, last ) ;
		SEXP x = PROTECT( Rf_allocVector( LGLSXP, size ) ) ;
		// FIXME : actually coerce
		// std::transform( first, last, LOGICAL(x), coerce_to_logical ) ;
		std::copy( first, last, LOGICAL(x) ); 
		setSEXP(x) ;
		UNPROTECT( 1 ); /* x */
	}
	
	
} ;

} // namespace

#endif
