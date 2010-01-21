// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// VectorBase.h: Rcpp R/C++ interface class library -- base class for all vectors
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

#ifndef Rcpp_VectorBase_h
#define Rcpp_VectorBase_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>

namespace Rcpp{ 

class VectorBase : public RObject {     
public:
	
    VectorBase() ;
    virtual ~VectorBase() = 0;
	
    /**
     * the length of the vector, uses Rf_length
     */
    inline int length() const { return ::Rf_length( m_sexp ) ; }
	
    /**
     * alias of length
     */
    inline int size() const { return ::Rf_length( m_sexp ) ; }
	
} ;

} // namespace

#endif
