// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RawVector.h: Rcpp R/C++ interface class library -- integer vectors
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

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/RawVector.h>
#include <algorithm>

namespace Rcpp{
	
	RawVector::RawVector(SEXP x) throw(not_compatible) : RObject() {
		switch( TYPEOF( x ) ){
			case RAWSXP:
				setSEXP( x ) ;
				break ;
			case INTSXP:
			case REALSXP:
			case LGLSXP:
				setSEXP( Rf_coerceVector( x, RAWSXP) ) ;
				break ;
			default:
				throw not_compatible( "cannot convert to intrger vector" ) ;
		}
	}
	
	RawVector::RawVector(int size) : RObject() {
		setSEXP( Rf_allocVector(RAWSXP, size) ) ;
	}

#ifdef HAS_INIT_LISTS
	RawVector::RawVector( std::initializer_list<int> list ) {
		SEXP x = PROTECT( Rf_allocVector( RAWSXP, list.size() ) ) ;
		std::copy( list.begin(), list.end(), RAW(x) ); 
		setSEXP(x) ;
		UNPROTECT( 1 ); /* x */
	}
	RawVector::RawVector( std::initializer_list<Rbyte> list ) {
		/* FIXME: we need to take care of coercion, so 
		transform is probably better */
		SEXP x = PROTECT( Rf_allocVector( RAWSXP, list.size() ) ) ;
		std::copy( list.begin(), list.end(), RAW(x) ); 
		setSEXP(x) ;
		UNPROTECT( 1 ); /* x */
	}
#endif

Rbyte& RawVector::operator[]( int i ) const throw(index_out_of_bounds){ 
	if( i<0 || i>= length() ) throw index_out_of_bounds() ;
	return RAW(m_sexp)[i] ;
}
Rbyte* RawVector::begin() const { 
	return RAW(m_sexp) ;
}
Rbyte* RawVector::end() const { 
	return RAW(m_sexp) + LENGTH(m_sexp);
}

} // namespace 
