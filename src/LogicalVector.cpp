// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// LogicalVector.h: Rcpp R/C++ interface class library -- integer vectors
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
#include <Rcpp/LogicalVector.h>
#include <algorithm>

namespace Rcpp{
	
	LogicalVector::LogicalVector(SEXP x) throw(not_compatible) : VectorBase() {
		switch( TYPEOF( x ) ){
			case LGLSXP:
				setSEXP( x ) ;
				break ;
			case RAWSXP:
			case INTSXP:
			case REALSXP:
				setSEXP( Rf_coerceVector( x, LGLSXP) ) ;
				break ;
			default:
				throw not_compatible( "cannot convert to intrger vector" ) ;
		}
	}
	
	LogicalVector::LogicalVector(int size) : VectorBase() {
		setSEXP( Rf_allocVector(LGLSXP, size) ) ;
	}

} // namespace 
