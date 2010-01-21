// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Function.cpp: Rcpp R/C++ interface class library -- functions
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

#include <Rcpp/Function.h>
#include <Rcpp/RObject.h>
#include <Rcpp/Language.h>
#include <Rcpp/Pairlist.h>
#include <RcppCommon.h>
#include <Rcpp/as.h>

namespace Rcpp {
	
	const char* Function::not_a_closure::what() const throw(){
		return "not a closure" ; 
	}
	
	Function::Function( SEXP x = R_NilValue ) throw(not_compatible) : RObject::RObject( ){
		switch( TYPEOF(x) ){
		case CLOSXP:
		case SPECIALSXP:
		case BUILTINSXP:
			setSEXP(x); 
			break; 
		default:
			throw not_compatible("cannot convert to function") ;
		}
	};
	
	Function::~Function(){}	
	
	Environment Function::environment() const throw(not_a_closure){
		if( TYPEOF(m_sexp) != CLOSXP ) {
			throw not_a_closure() ;
		}
		return Environment( CLOENV(m_sexp) ) ;
	}
	
} // namespace Rcpp
