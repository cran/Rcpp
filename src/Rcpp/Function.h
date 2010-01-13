// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Function.h: Rcpp R/C++ interface class library -- functions (also primitives and builtins)
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

#ifndef Rcpp_Function_h
#define Rcpp_Function_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/Pairlist.h>
#include <Rcpp/Evaluator.h>

namespace Rcpp{ 

/** 
 * functions
 */
class Function : public RObject{
public:

	/**
	 * thrown when attempting to get/set the environment of 
	 * a function that is a not a closure (CLOSXP)
	 */
	class not_a_closure : public std::exception{
	public:
		not_a_closure() throw() {} ;
		~not_a_closure() throw() {} ;
		const char* what() throw() ;
	} ;
	
	/**
	 * exception generated when a function calls generates an R error
	 */
	class eval_error : public std::exception{
	public:
		eval_error(const RObject& err) throw() ;
		~eval_error() throw() ;
		const char* what() throw() ;
	private: 
		std::string message ;
	} ;
	
	/**
	 * Attempts to convert the SEXP to a pair list
	 *
	 * @throw not_compatible if the SEXP could not be converted
	 * to a pair list using as.pairlist
	 */
	Function(SEXP lang) throw(not_compatible) ;


	/**
	 * calls the function with the specified arguments
	 *
	 * @param ...Args variable length argument list. The type of each 
	 *        argument must be wrappable, meaning there need to be 
	 *        a wrap function that takes this type as its parameter
	 *
	 */
#ifdef HAS_VARIADIC_TEMPLATES
template<typename... Args> 
	SEXP operator()( const Args&... args) throw(eval_error){
		
		/* FIXME: we should use applyClosure instead */
		Evaluator evaluator( Rf_lcons( m_sexp, pairlist(args...) ) ) ; 
		evaluator.run() ;
		if( evaluator.successfull() ){
			return evaluator.getResult() ;
		} else{
			throw eval_error( evaluator.getError() );
		}
	}
#endif
	
	/**
	 * Returns the environment of this function
	 */
	Environment environment() const throw(not_a_closure) ;
	
	~Function() ;
};

} // namespace Rcpp

#endif