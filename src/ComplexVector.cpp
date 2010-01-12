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

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/ComplexVector.h>
#include <algorithm>

namespace Rcpp{
	
	ComplexVector::ComplexVector(SEXP x) throw(not_compatible) : RObject() {
		switch( TYPEOF( x ) ){
			case CPLXSXP:
				setSEXP( x ) ;
				break ;
			case REALSXP:
			case LGLSXP:
			case RAWSXP:
			case INTSXP:
				setSEXP( Rf_coerceVector( x, CPLXSXP) ) ;
				break ;
			default:
				throw not_compatible( "cannot convert to complex vector" ) ;
		}
	}
	
	ComplexVector::ComplexVector(int size) : RObject() {
		setSEXP( Rf_allocVector(CPLXSXP, size) ) ;
	}

#ifdef HAS_INIT_LISTS	
	ComplexVector::ComplexVector( std::initializer_list<Rcomplex> list ) {
		SEXP x = PROTECT( Rf_allocVector( CPLXSXP, list.size() ) ) ;
		std::copy( list.begin(), list.end(), COMPLEX(x) ); 
		setSEXP(x) ;
		UNPROTECT( 1 ); /* x */
	}
#endif

Rcomplex& ComplexVector::operator[]( int i ) const throw(index_out_of_bounds){ 
	if( i<0 || i>= length()) throw index_out_of_bounds() ;
	return COMPLEX(m_sexp)[i] ;
}
Rcomplex* ComplexVector::begin() const { 
	return COMPLEX(m_sexp) ;
}
Rcomplex* ComplexVector::end() const { 
	return COMPLEX(m_sexp) + LENGTH(m_sexp);
}

} // namespace 
