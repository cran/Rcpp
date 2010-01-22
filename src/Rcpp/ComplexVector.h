// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// ComplexVector.h: Rcpp R/C++ interface class library -- complex vectors
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

#ifndef Rcpp_ComplexVector_h
#define Rcpp_ComplexVector_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/VectorBase.h>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#include <algorithm>
#endif

namespace Rcpp{ 

class ComplexVector : public VectorBase {     
public:

	ComplexVector(SEXP x) throw(not_compatible);
	ComplexVector(int size) ;
	
#ifdef HAS_INIT_LISTS	
	ComplexVector( std::initializer_list<Rcomplex> list ) : VectorBase(){
		fill( list.begin(), list.end() ) ;
	};
#endif
	
	inline Rcomplex& operator[]( int i ) const { return start[i] ; } 
	inline Rcomplex* begin() const { return start ; } 
	inline Rcomplex* end() const { return start + LENGTH(m_sexp) ; }
	
	typedef Rcomplex* iterator ;

private:
	Rcomplex* start ;
	virtual void update(){ start = COMPLEX(m_sexp);}
	
	template <typename InputIterator>
	void fill( InputIterator first, InputIterator last){
		size_t size = std::distance(first, last) ;
		SEXP x = PROTECT( Rf_allocVector( CPLXSXP, size ) ) ;
		std::copy( first, last, COMPLEX(x) ) ;
		setSEXP(x) ;
		UNPROTECT( 1 ); /* x */
	}
} ;

} // namespace

#endif
