// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// SEXP_Vector.h: Rcpp R/C++ interface class library -- template for expression vector and generic vector
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

#ifndef Rcpp_SEXP_Vector_h
#define Rcpp_SEXP_Vector_h

#include <RcppCommon.h>
#include <Rcpp/VectorBase.h>
#include <Rcpp/Environment.h>
#include <Rcpp/Dimension.h>

namespace Rcpp{

template <int RTYPE> 
class SEXP_Vector : public VectorBase {
public:
	
	/* much inspired from item 30 of more effective C++ */
	class Proxy {
	public:
		Proxy( SEXP_Vector& v, size_t i ) : parent(v), index(i) {}
		
		/* lvalue uses */
		Proxy& operator=(const Proxy& rhs){
			SET_VECTOR_ELT( parent, index, VECTOR_ELT( rhs.parent, rhs.index) ) ;
			return *this ;
		}
		
		Proxy& operator=(SEXP rhs){
			SET_VECTOR_ELT( parent, index, rhs ) ;
			return *this ;
		}
		
		template <typename T>
		Proxy& operator=( const T& rhs){
			SET_VECTOR_ELT( parent, index, wrap(rhs) ) ;
			return *this; 
		}
		
		/* rvalue use */
		operator SEXP() {
			return VECTOR_ELT( parent, index ) ; 
		}
		
		template <typename U> operator U(){
			SEXP xx = VECTOR_ELT( parent, index) ;
			return as<U>( xx ) ;
		}
		
		
	private:
		SEXP_Vector& parent; 
		size_t index ;
	} ;

	SEXP_Vector(): VectorBase(){}
	
	SEXP_Vector(SEXP x) : VectorBase() {
		SEXP y = r_cast<RTYPE>(x) ;
		setSEXP( y );
	}
	
	SEXP_Vector(const size_t& size) : VectorBase(){
		setSEXP( Rf_allocVector( RTYPE, size ) ) ;
	}
	
	SEXP_Vector(const Dimension& dims) : VectorBase(){
		setSEXP( Rf_allocVector( RTYPE, dims.prod() ) ) ;
		if( dims.size() > 1) attr( "dim" ) = dims ;
	}

	template <typename InputIterator>
	SEXP_Vector(InputIterator first, InputIterator last) : VectorBase() {
		assign( first, last ) ;
	}
	
#ifdef HAS_INIT_LISTS
	SEXP_Vector( std::initializer_list<SEXP> list) : VectorBase(){
		assign( list.begin(), list.end() ) ;
	} ;
#endif
	
	const Proxy operator[]( int i ) const throw(index_out_of_bounds){
		return Proxy(const_cast<SEXP_Vector<RTYPE>&>(*this), offset(i)) ;
	}
	Proxy operator[]( int i ) throw(index_out_of_bounds){
		return Proxy(*this, offset(i) ) ; 
	}

	
	Proxy operator()( const size_t& i) throw(index_out_of_bounds){
		return Proxy(*this, offset(i) ) ;
	}
	Proxy operator()( const size_t& i, const size_t& j) throw(index_out_of_bounds,not_a_matrix){
		return Proxy(*this, offset(i,j) ) ;
	}
	
	friend class Proxy; 
	
	template <typename InputIterator>
	void assign( InputIterator first, InputIterator last){
		size_t size = std::distance( first, last );
		SEXP x = PROTECT( Rf_allocVector( RTYPE, size ) ) ;
		SEXP y = R_NilValue ; /* -Wall */
		for( size_t i=0; i<size ; i++, ++first){
			/* this is where the actual type of InputIterator matters */
			y = *first ; /* as long as *first can implicitely convert to SEXP, we're good to go */
			SET_VECTOR_ELT( x, i, y ) ;
		}
		setSEXP( x ) ;
		UNPROTECT( 1 ); /* x */
	}
	
} ;
	
} //namespace Rcpp

#endif
