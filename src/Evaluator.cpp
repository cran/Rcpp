// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Evaluator.cpp: Rcpp R/C++ interface class library -- evaluator
//
// Copyright (C) 2009 - 2010	Dirk Eddelbuettel and Romain Francois
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

#include <Rcpp/Evaluator.h>
#include <Rcpp/Environment.h>

namespace Rcpp {
	
    Evaluator::Evaluator( SEXP expression = R_NilValue) : 
	expression(expression),
	error_occured(false), 
	result(),
	error() {}
	
    Evaluator::~Evaluator(){} 
	
    void Evaluator::run(SEXP env ) throw() {
	Environment rcpp = Environment::namespace_env("Rcpp") ;
	SEXP call = Rf_lang3( Rf_install("protectedEval"), expression, env ) ;
	result = wrap( Rf_eval( call, rcpp ) ); 
	error_occured = LOGICAL( Rf_eval( Rf_lang1( Rf_install("errorOccured")) , rcpp) )[0] ;
	if( error_occured ){
	    error = wrap( Rf_eval( Rf_lang1(Rf_install("getCurrentError")) , rcpp) );
	}
    }
    
    void Evaluator::run() throw() {
    	run( R_GlobalEnv) ;
    }

} // namespace Rcpp
