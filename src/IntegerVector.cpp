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

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/IntegerVector.h>
#include <algorithm>

namespace Rcpp{
	
	IntegerVector::IntegerVector(SEXP x) throw(not_compatible) : RObject() {
		switch( TYPEOF( x ) ){
			case INTSXP:
				setSEXP( x ) ;
				break ;
			case REALSXP:
			case LGLSXP:
			case RAWSXP:
				setSEXP( Rf_coerceVector( x, INTSXP) ) ;
				break ;
			default:
				throw not_compatible( "cannot convert to intrger vector" ) ;
		}
	}
	
	IntegerVector::IntegerVector(int size) : RObject() {
		setSEXP( Rf_allocVector(INTSXP, size) ) ;
	}

#ifdef HAS_INIT_LISTS	
	IntegerVector::IntegerVector( std::initializer_list<int> list ) {
		SEXP x = PROTECT( Rf_allocVector( INTSXP, list.size() ) ) ;
		std::copy( list.begin(), list.end(), INTEGER(x) ); 
		setSEXP(x) ;
		UNPROTECT( 1 ); /* x */
	}
	IntegerVector::IntegerVector( std::initializer_list<double> list ) {
		SEXP x = PROTECT( Rf_allocVector( INTSXP, list.size() ) ) ;
		std::copy( list.begin(), list.end(), INTEGER(x) ); 
		setSEXP(x) ;
		UNPROTECT( 1 ); /* x */
	}
#endif

int& IntegerVector::operator[]( int i ) const throw(index_out_of_bounds) { 
	if( i < 0 || i >= length() ) throw index_out_of_bounds() ;
	return INTEGER(m_sexp)[i] ;
}
int* IntegerVector::begin() const { 
	return INTEGER(m_sexp) ;
}
int* IntegerVector::end() const { 
	return INTEGER(m_sexp) + LENGTH(m_sexp);
}

} // namespace 
