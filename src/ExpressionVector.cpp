// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// ExpressionVector.cpp: Rcpp R/C++ interface class library -- expression vectors
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
#include <Rcpp/ExpressionVector.h>
#include <Rcpp/Evaluator.h>
#include <Rcpp/Symbol.h>
#include <algorithm>

namespace Rcpp{
	
	ExpressionVector::parse_error::parse_error() throw(){}
	ExpressionVector::parse_error::~parse_error() throw(){}
	const char* ExpressionVector::parse_error::what() throw(){ return "parse error" ; }
	
	ExpressionVector::ExpressionVector(SEXP x) throw(not_compatible) : VectorBase() {
		switch( TYPEOF( x ) ){
			case EXPRSXP:
				setSEXP( x ) ;
				break ;
			default:
				{
					SEXP res = R_NilValue ;
					try{
						res = Evaluator::run( Rf_lang2( Rf_install("as.expression"), x ) ) ;
					} catch( const Evaluator::eval_error& e){
						throw not_compatible( "could not convert to an expression vector" ) ;
					}
					setSEXP( res ) ;
				}
		}
	}
	
	ExpressionVector::ExpressionVector(int size) : VectorBase() {
		setSEXP( Rf_allocVector(EXPRSXP, size) ) ;
	}

	ExpressionVector::ExpressionVector(const std::string& code) throw(parse_error){
		ParseStatus status;
		SEXP expr = PROTECT( Rf_mkString( code.c_str() ) );
		SEXP res  = PROTECT( R_ParseVector(expr, -1, &status, R_NilValue));
		switch( status ){
		case PARSE_OK:
			setSEXP( res) ;
			UNPROTECT( 2) ;
			break;
		default:
			UNPROTECT(2) ;
			throw parse_error() ;
		}
	}

ExpressionVector::Proxy::Proxy(ExpressionVector& v, int i) :
	parent(v), index(i){}

ExpressionVector::Proxy::operator SEXP() const{
	return VECTOR_ELT( parent, index ) ;
}

ExpressionVector::Proxy& ExpressionVector::Proxy::operator=( const Proxy& rhs){
	SET_VECTOR_ELT( parent, index, VECTOR_ELT( rhs.parent, rhs.index) ) ;
	return *this ;
}

ExpressionVector::Proxy& ExpressionVector::Proxy::operator=( SEXP rhs){
	SET_VECTOR_ELT( parent, index, rhs ) ;
	return *this ;
}

const ExpressionVector::Proxy ExpressionVector::operator[](int i) const throw(index_out_of_bounds){
	if( i<0 || i>= length()) throw index_out_of_bounds() ;
	return Proxy(const_cast<ExpressionVector&>(*this), i) ;
}

ExpressionVector::Proxy ExpressionVector::operator[](int i) throw(index_out_of_bounds){
	if( i<0 || i>= length()) throw index_out_of_bounds() ;
	return Proxy(*this, i ) ;
}

SEXP ExpressionVector::eval() throw(Evaluator::eval_error){
	return Evaluator::run( Rf_lcons( Rf_install( "eval" ) , Rf_cons( m_sexp, R_NilValue) )) ;
}

SEXP ExpressionVector::eval(const Environment& env) throw(Evaluator::eval_error){
	return Evaluator::run( Rf_lcons( Rf_install( "eval" ) , Rf_cons( m_sexp, Rf_cons(env.asSexp(), R_NilValue)) ) ) ;
}

} // namespace 
