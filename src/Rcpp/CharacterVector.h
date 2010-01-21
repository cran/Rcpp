// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// CharacterVector.h: Rcpp R/C++ interface class library -- character vectors
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

#ifndef Rcpp_CharacterVector_h
#define Rcpp_CharacterVector_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/VectorBase.h>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#include <algorithm>
#endif

namespace Rcpp{ 

class CharacterVector : public VectorBase {     
public:

	/* much inspired from item 30 of more effective C++ */
	class StringProxy {
	public:
		StringProxy( CharacterVector& v, int index ) ;
		
		/* lvalue uses */
		StringProxy& operator=(const StringProxy& rhs) ;
		StringProxy& operator=(const std::string& rhs) ;
		
		StringProxy& operator+=(const StringProxy& rhs) ;
		StringProxy& operator+=(const std::string& rhs) ;
		
		/* rvalue use */
		operator SEXP() const ;
		operator char*() const ;
		
	private:
		CharacterVector& parent; 
		int index ;
	} ;

	CharacterVector(SEXP x) throw(not_compatible);
	CharacterVector(int size) ;
	CharacterVector( const std::string& x );
	CharacterVector( const std::vector<std::string>& x );
	
#ifdef HAS_INIT_LISTS
	CharacterVector( std::initializer_list<std::string> list ) : VectorBase() {
		fill( list.begin(), list.size() ) ;
	}
#endif

	const StringProxy operator[]( int i ) const throw(index_out_of_bounds);
	StringProxy operator[]( int i ) throw(index_out_of_bounds);

	friend class StringProxy; 

private:
	template <typename InputIterator>
	void fill(InputIterator first, size_t size ){
		SEXP x = PROTECT( Rf_allocVector( STRSXP, size) ) ;
		for( size_t i=0; i<size; ++i, ++first ){
			SET_STRING_ELT( x, i, Rf_mkChar(first->c_str()) ) ;
		}
		setSEXP( x );
		UNPROTECT(1) ;
	}
} ;

typedef CharacterVector StringVector ;

} // namespace

#endif
