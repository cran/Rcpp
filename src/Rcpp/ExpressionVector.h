// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// ExpressionVector.h: Rcpp R/C++ interface class library -- expression vectors
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

#ifndef Rcpp_ExpressionVector_h
#define Rcpp_ExpressionVector_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#include <algorithm>
#endif

namespace Rcpp{ 

class ExpressionVector : public RObject {     
public:

	/* much inspired from item 30 of more effective C++ */
	class Proxy {
	public:
		Proxy( ExpressionVector& v, int index ) ;
		
		/* lvalue uses */
		Proxy& operator=(const Proxy& rhs) ;
		Proxy& operator=(SEXP rhs) ;

		template <typename T>
		Proxy& operator=( const T& rhs){
			SET_VECTOR_ELT( parent, index, wrap(rhs) ) ;
			return *this; 
		}

		/* rvalue use */
		operator SEXP() const ;

	private:
		ExpressionVector& parent; 
		int index ;
	} ;

	ExpressionVector(SEXP x) throw(not_compatible);
	ExpressionVector( int size) ;
	
#ifdef HAS_INIT_LISTS	
	ExpressionVector( std::initializer_list<RObject> list ) ;
#endif

	/**
	 * the length of the vector, uses Rf_length
	 */
	inline int length() const { return Rf_length( m_sexp ) ; }

	/**
	 * alias of length
	 */
	inline int size() const { return Rf_length( m_sexp ) ; }

	SEXP* begin(); 
	SEXP* end() ;

	const Proxy operator[]( int i ) const throw(index_out_of_bounds);
	Proxy operator[]( int i ) throw(index_out_of_bounds) ;

	friend class Proxy; 

} ;

} // namespace

#endif
