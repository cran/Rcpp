// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// VectorBase.h: Rcpp R/C++ interface class library -- base vector class
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
#include <Rcpp/VectorBase.h>

namespace Rcpp{
	
	VectorBase::VectorBase() : RObject() {} ;
	VectorBase::~VectorBase(){}
	
	size_t VectorBase::offset( const size_t& i, const size_t& j) const throw(not_a_matrix,RObject::index_out_of_bounds) {
		if( !Rf_isMatrix(m_sexp) ) throw VectorBase::not_a_matrix() ;
		/* we need to extract the dimensions */
		int *dim = INTEGER( Rf_getAttrib( m_sexp, R_DimSymbol ) ) ;
		size_t nrow = static_cast<size_t>(dim[0]) ;
		size_t ncol = static_cast<size_t>(dim[1]) ;
		if( i >= nrow || j >= ncol ) throw RObject::index_out_of_bounds() ;
		return i + nrow*j ;
	}

	size_t VectorBase::offset(const size_t& i) const throw(RObject::index_out_of_bounds){
    	    if( i >= static_cast<size_t>(Rf_length(m_sexp)) ) throw RObject::index_out_of_bounds() ;
    	    return i ;
    	}

} // namespace 
