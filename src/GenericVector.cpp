// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// GenericVector.cpp: Rcpp R/C++ interface class library -- generic vectors (lists)
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
#include <Rcpp/GenericVector.h>
#include <Rcpp/Evaluator.h>
#include <Rcpp/Symbol.h>
#include <algorithm>

namespace Rcpp{
	
	GenericVector::GenericVector(SEXP x) throw(not_compatible) : RObject() {
		switch( TYPEOF( x ) ){
			case VECSXP:
				setSEXP( x ) ;
				break ;
			default:
				{
					Evaluator e( Rf_lang2( Symbol("as.list"), x ) ) ;
					e.run() ;
					if( e.successfull() ){
						setSEXP( e.getResult() ) ;
					} else{
						throw not_compatible( "could not convert to a list" ) ;
					}
				}
		}
	}
	
	GenericVector::GenericVector(int size) : RObject() {
		setSEXP( Rf_allocVector(VECSXP, size) ) ;
	}

#ifdef HAS_INIT_LISTS
	GenericVector::GenericVector( std::initializer_list<RObject> list ) {
		SEXP x = PROTECT( Rf_allocVector( VECSXP, list.size() ) ) ;
		const RObject* p = list.begin() ;
		for( size_t i=0; i<list.size() ; i++, p++){
			SET_VECTOR_ELT( x, i, p->asSexp() ) ;
		}
		setSEXP( x ) ;
		UNPROTECT( 1 ); /* x */
	}
#endif

// SEXP* GenericVector::begin(){
// 	return RCPP_VECTOR_PTR(m_sexp) ;
// }
// 
// SEXP* GenericVector::end(){
// 	return RCPP_VECTOR_PTR(m_sexp) + LENGTH(m_sexp) ;
// }

/* proxy stuff */

GenericVector::Proxy::Proxy(GenericVector& v, int i) :
	parent(v), index(i){}

GenericVector::Proxy::operator SEXP() const{
	return VECTOR_ELT( parent, index ) ;
}

GenericVector::Proxy& GenericVector::Proxy::operator=( const Proxy& rhs){
	SET_VECTOR_ELT( parent, index, VECTOR_ELT( rhs.parent, rhs.index) ) ;
	return *this ;
}

GenericVector::Proxy& GenericVector::Proxy::operator=( SEXP rhs){
	SET_VECTOR_ELT( parent, index, rhs ) ;
	return *this ;
}

const GenericVector::Proxy GenericVector::operator[](int i) const throw(index_out_of_bounds){
	if( i<0 || i>=length()) throw index_out_of_bounds() ;
	return Proxy(const_cast<GenericVector&>(*this), i) ;
}

GenericVector::Proxy GenericVector::operator[](int i) throw(index_out_of_bounds){
	if( i<0 || i>=length()) throw index_out_of_bounds() ;
	return Proxy(*this, i ) ;
}


} // namespace 