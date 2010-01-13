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

#include <Rcpp/Pairlist.h>
#include <Rcpp/Evaluator.h>
#include <Rcpp/RObject.h>
#include <Rcpp/Language.h>
#include <RcppCommon.h>

namespace Rcpp {

	Pairlist::Pairlist( SEXP x = R_NilValue ) throw(not_compatible) : RObject::RObject( ){
		if( x != R_NilValue ){
			switch( TYPEOF(x) ){
				case LANGSXP:
				case LISTSXP:
					setSEXP( x) ; 
					break ;
				default:
					{
						Evaluator evaluator( Rf_lang2( Rf_install("as.pairlist"), x ) ) ;
						evaluator.run() ;
						if( evaluator.successfull() ){
    							setSEXP( evaluator.getResult().asSexp() ) ;
    						} else{
    							throw not_compatible( "cannot convert to call (LANGSXP)" ) ; 
    						}
					}
			}
		}          
	};

	Pairlist::~Pairlist(){}

	void Pairlist::remove( const int& index ) throw(index_out_of_bounds){
		if( index < 0 || index >= Rf_length(m_sexp) ) throw index_out_of_bounds() ;
		if( index == 0 ){
			setSEXP( CDR( m_sexp) ) ;
		} else{
			SEXP x = m_sexp ;
			int i=1;
			while( i<index ){ x = CDR(x) ; i++; }
			SETCDR( x, CDDR(x) ) ;
		}
	}

	Pairlist::Proxy::Proxy(Pairlist& v, const size_t& index) :
		parent(v), index(index) {} ;
	
	Pairlist::Proxy& Pairlist::Proxy::operator=(const Proxy& rhs){
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
		SET_TAG( target, TAG(origin) ); 
		return *this ;
	}
	
	Pairlist::Proxy& Pairlist::Proxy::operator=(const Named& rhs){
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		size_t i = 0 ;
		SEXP x = parent.asSexp() ; 
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		SETCAR( x, rhs.getSEXP() ) ;
		SET_TAG( x, Symbol( rhs.getTag() ) ) ;
		return *this ;
	}
	
	Pairlist::Proxy& Pairlist::Proxy::operator=(SEXP rhs){
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
	
	Pairlist::Proxy::operator SEXP() const{
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		SEXP x = parent.asSexp() ; 
		size_t i = 0 ;
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		return CAR(x) ;
	}
	
	Pairlist::Proxy::operator RObject() const{
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		SEXP x = parent.asSexp() ; 
		size_t i = 0 ;
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		return wrap( CAR(x) ) ;
	}
	
	const Pairlist::Proxy Pairlist::operator[](int i) const {
		return Proxy( const_cast<Pairlist&>(*this), i) ;
	}
	
	Pairlist::Proxy Pairlist::operator[](int i){
		return Proxy( *this, i );
	}
	
	
	SEXP pairlist(){ return R_NilValue ; }

} // namespace Rcpp
