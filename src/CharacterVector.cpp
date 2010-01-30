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

#include <Rcpp/CharacterVector.h>

namespace Rcpp{

CharacterVector::CharacterVector() : VectorBase(){}
	
CharacterVector::CharacterVector(SEXP x) throw(not_compatible) : VectorBase() {
	SEXP y = r_cast<STRSXP>( x) ;
	setSEXP( y ) ;
}

CharacterVector::CharacterVector(const size_t& size) : VectorBase(){
	setSEXP( Rf_allocVector( STRSXP, size ) ) ;
}

CharacterVector::CharacterVector( const std::string& x) : VectorBase() {
	setSEXP( Rf_mkString(x.c_str()) ) ;
}

CharacterVector::CharacterVector( const std::vector<std::string>& x): VectorBase() {
	assign( x.begin(), x.end() ) ;
}

CharacterVector::CharacterVector( const Dimension& dims): VectorBase(){
	setSEXP( Rf_allocVector( STRSXP, dims.prod() ) ) ;
	if( dims.size() > 1 ) attr( "dim" ) = dims ;
}

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

std::ostream& operator<<(std::ostream& os, const CharacterVector::StringProxy& proxy) {
    os << CHAR(STRING_ELT( proxy.parent, proxy.index )) ;
    return os;
}

const CharacterVector::StringProxy CharacterVector::operator[](int i) const throw(index_out_of_bounds){
	return StringProxy(const_cast<CharacterVector&>(*this), offset(i) ) ;
}                                          

CharacterVector::StringProxy CharacterVector::operator[](int i) throw(index_out_of_bounds) {
	return StringProxy(*this, offset(i) ) ;
}

CharacterVector::StringProxy CharacterVector::operator()( const size_t& i) throw(index_out_of_bounds){
	return StringProxy(*this, offset(i) ) ;
}

CharacterVector::StringProxy CharacterVector::operator()( const size_t& i, const size_t&j ) throw(index_out_of_bounds,not_a_matrix){
	return StringProxy(*this, offset(i,j) ) ;
}

} // namespace 
