// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// as.h: Rcpp R/C++ interface class library -- generic converters from SEXP
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

#ifndef Rcpp_as_h
#define Rcpp_as_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>

namespace Rcpp{ 

/** 
 * Generic converted from SEXP to the typename
 */
template <typename T> T as( SEXP m_sexp) {
	throw std::runtime_error("not implemented") ; 
}
template<> bool 			as<bool>(SEXP m_sexp) ;
template<> double                   	as<double>(SEXP m_sexp) ;
template<> int                      	as<int>(SEXP m_sexp) ;
template<> Rbyte                    	as<Rbyte>(SEXP m_sexp) ;
template<> std::string              	as<std::string>(SEXP m_sexp) ;
template<> std::vector<int>         	as< std::vector<int> >(SEXP m_sexp) ;
template<> std::vector<double>      	as< std::vector<double> >(SEXP m_sexp) ;
template<> std::vector<std::string> 	as< std::vector<std::string> >(SEXP m_sexp) ;
template<> std::vector<Rbyte>       	as< std::vector<Rbyte> >(SEXP m_sexp) ;
template<> std::vector<bool>        	as< std::vector<bool> >(SEXP m_sexp) ;

} // namespace Rcpp      

#endif
