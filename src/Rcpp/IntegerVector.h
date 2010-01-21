// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// IntegerVector.h: Rcpp R/C++ interface class library -- integer vectors
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

#ifndef Rcpp_IntegerVector_h
#define Rcpp_IntegerVector_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/VectorBase.h>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#include <algorithm>
#endif

namespace Rcpp{ 

class IntegerVector : public VectorBase {     
public:

	IntegerVector(SEXP x) throw(not_compatible);
	IntegerVector(int size) ;
	
#ifdef HAS_INIT_LISTS	
	IntegerVector( std::initializer_list<int> list ): VectorBase(), start(0) { 
		simple_fill( list.begin(), list.end() );
	}
	IntegerVector( std::initializer_list<double> list ) :VectorBase(), start(0) { 
		coerce_and_fill( list.begin(), list.end() );
	}
#endif

	inline int& operator[]( int i ) const{ return start[i] ; }
	inline int* begin() const { return start ; }
	inline int* end() const { return start + LENGTH(m_sexp) ; }
	
	typedef int* iterator ;

private:
	int* start ;
	virtual void update(){ start = INTEGER(m_sexp) ;}
	
	// simple is when there is no need for coercion
	template <typename InputIterator>
	void simple_fill( InputIterator first, InputIterator last){
		size_t size = std::distance( first, last) ;
		SEXP x = PROTECT( Rf_allocVector( INTSXP, size ) );
		std::copy( first, last, INTEGER(x) ) ;
		setSEXP( x ) ;
		UNPROTECT(1) ;
	}
	
	// need to coerce to int
	template <typename InputIterator>
	void coerce_and_fill( InputIterator first, InputIterator last){
		size_t size = std::distance( first, last) ;
		SEXP x = PROTECT( Rf_allocVector( INTSXP, size ) );
		// FIXME: actually coerce
		// std::transform( first, last, INTEGER(x), coerce<int> ) ;
		std::copy( first, last, INTEGER(x) ) ;
		setSEXP( x ) ;
		UNPROTECT(1) ;
	}

} ;

} // namespace

#endif
