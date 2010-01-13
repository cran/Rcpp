// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// CharacterVector.cpp: Rcpp R/C++ interface class library -- character vectors
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
#include <Rcpp/CharacterVector.h>
#include <algorithm>

namespace Rcpp{
	
	CharacterVector::CharacterVector(SEXP x) throw(not_compatible) : RObject() {
		switch( TYPEOF( x ) ){
			case STRSXP:
				setSEXP( x ) ;
				break ;
			case SYMSXP:
				setSEXP( Rf_ScalarString(PRINTNAME(x)) ) ;
				break ;
			case CHARSXP:
				setSEXP( Rf_ScalarString( x ) ) ;
			default:
				/* TODO: try coercion */
				throw not_compatible( "not compatible with character vector" ) ;
		}
	}
	
	CharacterVector::CharacterVector(int size) : RObject() {
		setSEXP( Rf_allocVector(STRSXP, size) ) ;
	}
	
	CharacterVector::CharacterVector( const std::string& x){
		setSEXP( Rf_mkString(x.c_str()) ) ;
	}
	
	CharacterVector::CharacterVector( const std::vector<std::string>& x){
		SEXP y = PROTECT( Rf_allocVector( STRSXP, x.size() ) );
		int n = x.size() ;
		std::vector<std::string>::const_iterator iter = x.begin() ;
		for( int i=0; i<n; i++, iter++){
			SET_STRING_ELT( y, i, Rf_mkChar(iter->c_str()) ) ;
		}
		setSEXP(y) ;
		UNPROTECT(1) ;
	}
	
#ifdef HAS_INIT_LISTS
	CharacterVector::CharacterVector( std::initializer_list<std::string> list ) {
		SEXP x = PROTECT( Rf_allocVector( STRSXP, list.size() ) ) ;
		const std::string *p = list.begin() ;
		int n = list.size() ;
		for( int i=0; i<n ; i++, p++){
			SET_STRING_ELT( x, i, Rf_mkChar(p->c_str()) ) ;
		}
		setSEXP( x ) ;
		UNPROTECT( 1 ); /* x */
	}
#endif

SEXP CharacterVector::get( const int& i ) const { 
	return STRING_ELT(m_sexp, i) ;
}

void CharacterVector::set( const int& i, const std::string& value ){
	SET_STRING_ELT(m_sexp,i, Rf_mkChar(value.c_str()) ) ;
}

// SEXP* CharacterVector::begin(){
// 	return RCPP_VECTOR_PTR(m_sexp) ;
// }
// 
// SEXP* CharacterVector::end(){
// 	return RCPP_VECTOR_PTR(m_sexp) + LENGTH(m_sexp) ;
// }

/* proxy stuff */

CharacterVector::StringProxy::StringProxy(CharacterVector& v, int i) :
	parent(v), index(i){}

CharacterVector::StringProxy::operator SEXP() const{
	return STRING_ELT( parent, index ) ;
}

CharacterVector::StringProxy::operator char*() const {
	return const_cast<char*>( CHAR(STRING_ELT( parent, index )) );
}

CharacterVector::StringProxy& CharacterVector::StringProxy::operator=( const StringProxy& rhs){
	SET_STRING_ELT( parent, index, STRING_ELT( rhs.parent, rhs.index) ) ;
	return *this ;
}

CharacterVector::StringProxy& CharacterVector::StringProxy::operator+=( const std::string& rhs){
	std::string full( CHAR(STRING_ELT(parent,index)) ) ;
	full += rhs ;
	SET_STRING_ELT( parent, index, Rf_mkChar( full.c_str() ) ) ;
	return *this ;
}

CharacterVector::StringProxy& CharacterVector::StringProxy::operator+=( const StringProxy& rhs){
	std::string full( CHAR(STRING_ELT(parent,index)) ) ;
	full += CHAR(STRING_ELT( rhs.parent, rhs.index)) ;
	SET_STRING_ELT( parent, index, Rf_mkChar(full.c_str()) ) ;
	return *this ;
}

CharacterVector::StringProxy& CharacterVector::StringProxy::operator=( const std::string& rhs){
	SET_STRING_ELT( parent, index, Rf_mkChar( rhs.c_str() ) ) ;
	return *this ;
}


const CharacterVector::StringProxy CharacterVector::operator[](int i) const throw(index_out_of_bounds){
	if( i<0 || i>=length()) throw index_out_of_bounds() ;
	return StringProxy(const_cast<CharacterVector&>(*this), i) ;
}

CharacterVector::StringProxy CharacterVector::operator[](int i) throw(index_out_of_bounds) {
	if( i<0 || i>=length()) throw index_out_of_bounds() ;
	return StringProxy(*this, i ) ;
}



} // namespace 