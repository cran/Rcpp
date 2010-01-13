// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// NumericVector.h: Rcpp R/C++ interface class library -- integer vectors
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
#include <Rcpp/NumericVector.h>

namespace Rcpp{
	
	NumericVector::NumericVector(SEXP x) throw(not_compatible) : RObject() {
		switch( TYPEOF( x ) ){
			case REALSXP:
				setSEXP( x ) ;
				break ;
			case INTSXP:
			case LGLSXP:
			case RAWSXP:
				setSEXP( Rf_coerceVector( x, REALSXP) ) ;
				break ;
			default:
				throw not_compatible( "cannot convert to numeric vector" ) ;
		}
	}
	
	NumericVector::NumericVector(int size) : RObject() {
		setSEXP( Rf_allocVector(REALSXP, size) ) ;
	}

#ifdef HAS_INIT_LISTS	
	NumericVector::NumericVector( std::initializer_list<int> list ) {
		SEXP x = PROTECT( Rf_allocVector( REALSXP, list.size() ) ) ;
		std::copy( list.begin(), list.end(), REAL(x) ); 
		setSEXP(x) ;
		UNPROTECT( 1 ); /* x */
	}
	NumericVector::NumericVector( std::initializer_list<double> list ) {
		SEXP x = PROTECT( Rf_allocVector( REALSXP, list.size() ) ) ;
		std::copy( list.begin(), list.end(), REAL(x) ); 
		setSEXP(x) ;
		UNPROTECT( 1 ); /* x */
	}
#endif

double& NumericVector::operator[]( int i ) const throw(index_out_of_bounds){ 
	if( i<0 || i>=length()) throw index_out_of_bounds() ;
	return REAL(m_sexp)[i] ;
}
double* NumericVector::begin() const { 
	return REAL(m_sexp) ;
}
double* NumericVector::end() const { 
	return REAL(m_sexp) + LENGTH(m_sexp);
}

} // namespace 
