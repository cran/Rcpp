// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// GenericVector.h: Rcpp R/C++ interface class library -- generic vectors (lists)
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

#ifndef Rcpp_GenericVector_h
#define Rcpp_GenericVector_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/Environment.h>
#include <Rcpp/VectorBase.h>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#include <algorithm>
#endif

namespace Rcpp{ 

class GenericVector : public VectorBase {     
public:

	/* much inspired from item 30 of more effective C++ */
	class Proxy {
	public:
		Proxy( GenericVector& v, int index ) ;
		
		/* lvalue uses */
		Proxy& operator=(const Proxy& rhs) ;
		Proxy& operator=(SEXP rhs) ;
		
		template <typename T>
		Proxy& operator=( const T& rhs){
			SET_VECTOR_ELT( parent, index, wrap(rhs) ) ;
			return *this; 
		}
		Proxy& operator=( const Environment::Binding& rhs) ;
		
		/* rvalue use */
		operator SEXP() const ;
		
	private:
		GenericVector& parent; 
		int index ;
	} ;


	GenericVector(SEXP x) throw(not_compatible);
	GenericVector( int size) ;
	
#ifdef HAS_INIT_LISTS	
	GenericVector( std::initializer_list<RObject> list ) : VectorBase(){
		fill( list.begin(), list.end() ) ;
	};
#endif
	
	const Proxy operator[]( int i ) const throw(index_out_of_bounds);
	Proxy operator[]( int i ) throw(index_out_of_bounds) ;
	
	friend class Proxy; 

	template <typename InputIterator>
	void fill( InputIterator first, InputIterator last){
		size_t size = std::distance( first, last );
		SEXP x = PROTECT( Rf_allocVector( VECSXP, size ) ) ;
		for( size_t i=0; i<size ; i++, ++first){
			SET_VECTOR_ELT( x, i, *first ) ;
		}
		setSEXP( x ) ;
		UNPROTECT( 1 ); /* x */
	}
	
} ;

typedef GenericVector List ;

} // namespace

#endif
