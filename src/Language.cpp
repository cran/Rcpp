// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Language.cpp: Rcpp R/C++ interface class library -- Language objects ( calls )
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

#include <Rcpp/Language.h>
#include <RcppCommon.h>
#include <Rcpp/wrap.h>

namespace Rcpp {
	
	Language::Language( SEXP lang = R_NilValue ) throw(not_compatible) : RObject::RObject( ){
		/* if this is not trivially a call, then try to convert it to one */
		if( lang != R_NilValue && TYPEOF(lang) != LANGSXP ){
	    		
	    		/* taken from do_ascall */
	    		switch( TYPEOF(lang) ){
	    		case LISTSXP :
	    			Rf_duplicate( lang ) ;
	    			break ;
	    		case VECSXP:
	    		case EXPRSXP:
	    			{
	    				int n = Rf_length(lang) ;
	    				if( n == 0 ) throw not_compatible("cannot convert to call (LANGSXP)") ;
	    				SEXP names = RCPP_GET_NAMES(lang) ; 
	    				SEXP res, ap;
	    				PROTECT( ap = res = Rf_allocList( n ) ) ;
	    				for( int i=0; i<n; i++){
	    					SETCAR(ap, VECTOR_ELT(lang, i));
	    					if (names != R_NilValue && !Rf_StringBlank(STRING_ELT(names, i))){
	    						SET_TAG(ap, Rf_install(Rf_translateChar(STRING_ELT(names, i))));
	    					}
	    					ap = CDR( ap) ;
	    				}
	    				UNPROTECT(1) ;
	    				setSEXP(res) ; 
	    			}
	    		default:
	    			throw not_compatible("cannot convert to call (LANGSXP)") ;
	    		}
	    		SET_TYPEOF(m_sexp, LANGSXP);
	    		SET_TAG(m_sexp, R_NilValue);
		} else{
			setSEXP( lang ) ;
		}

	};
	
	Language::Language( const std::string& symbol ): RObject::RObject(R_NilValue) {
		setSEXP( Rf_lcons( Symbol(symbol), R_NilValue ) );
	}
	
	Language::Language( const Symbol& symbol ){
		setSEXP( Rf_lcons( symbol, R_NilValue ) ) ;
	}
	
	Language::~Language(){}

	void Language::setSymbol( const std::string& symbol){
		setSymbol( Symbol( symbol ) ) ;
	}
	
	void Language::setSymbol( const Symbol& symbol){
		SETCAR( m_sexp, symbol ) ;
		SET_TAG(m_sexp, R_NilValue);
	}
	
	void Language::remove( const int& index ) throw(index_out_of_bounds){
		if( index < 0 || index >= Rf_length(m_sexp) ) throw index_out_of_bounds() ;
		if( index == 0 ){
			setSEXP( CDR( m_sexp) ) ;
			SET_TAG(m_sexp, R_NilValue);
			SET_TYPEOF( m_sexp, LANGSXP ) ;
		} else{
			SEXP x = m_sexp ;
			int i=1;
			while( i<index ){ x = CDR(x) ; i++; }
			SETCDR( x, CDDR(x) ) ;
		}
	}

	
	/* proxy for operator[] */
	
	Language::Proxy::Proxy(Language& v, const size_t& index) :
		parent(v), index(index) {} ;
	
	Language::Proxy& Language::Proxy::operator=(const Proxy& rhs){
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		if( rhs.index < 0 || rhs.index >=  rhs.parent.length() ) throw index_out_of_bounds() ;
		
		SEXP target = parent.asSexp() ;
		SEXP origin = rhs.parent.asSexp();
		size_t i=0; 
		while( i < index ){
			target = CDR(target) ;
			i++; 
		}
		i=0; 
		while( i < rhs.index ){
			origin = CDR(origin) ;
			i++;
		}
		SETCAR( target, CAR(origin) );
		if( index != 0 ) SET_TAG( target, TAG(origin) ); 
		return *this ;
	}
	
	Language::Proxy& Language::Proxy::operator=(const Named& rhs){
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		size_t i = 0 ;
		SEXP x = parent.asSexp() ; 
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		SETCAR( x, rhs.getSEXP() ) ;
		if( index != 0 ) SET_TAG( x, Symbol( rhs.getTag() ) ) ;
		return *this ;
	}
	
	Language::Proxy& Language::Proxy::operator=(SEXP rhs){
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		SEXP x = parent.asSexp() ; 
		size_t i = 0 ;
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		SETCAR( x, rhs) ;
		return *this ;
	}
	
	
	/* rvalue uses */
	
	Language::Proxy::operator SEXP() const{
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		SEXP x = parent.asSexp() ; 
		size_t i = 0 ;
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		return CAR(x) ;
	}
	
	Language::Proxy::operator RObject() const{
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		SEXP x = parent.asSexp() ; 
		size_t i = 0 ;
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		return wrap( CAR(x) ) ;
	}
	
	const Language::Proxy Language::operator[](int i) const {
		return Proxy( const_cast<Language&>(*this), i) ;
	}
	
	Language::Proxy Language::operator[](int i){
		return Proxy( *this, i );
	}
	
	
	
	
} // namespace Rcpp
