// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// LogicalVector.h: Rcpp R/C++ interface class library -- logical vectors
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

#ifndef Rcpp_LogicalVector_h
#define Rcpp_LogicalVector_h

#include <RcppCommon.h>
#include <Rcpp/SimpleVector.h>

namespace Rcpp{

typedef SimpleVector<LGLSXP,int> LogicalVector_Base ;

class LogicalVector : public LogicalVector_Base{ 
public:
	LogicalVector() : LogicalVector_Base(){} ;
	LogicalVector(SEXP x) : LogicalVector_Base(x){} ;
	LogicalVector(const size_t& size) : LogicalVector_Base(size){}; 

#ifdef HAS_INIT_LISTS
	LogicalVector( std::initializer_list<int> list) : LogicalVector_Base(list){};
	LogicalVector( std::initializer_list<bool> list) : LogicalVector_Base(){ 
		bool_fill(list.begin(), list.end());
	} ;
#endif
	
private:
	template <typename InputIterator>
	void bool_fill( InputIterator first, InputIterator last){
		size_t size = std::distance( first, last) ;
		SEXP x = PROTECT( Rf_allocVector( LGLSXP, size ) );
		std::transform( first, last, get_pointer<LGLSXP,int>(x), bool_to_Rboolean  ) ;
		setSEXP( x ) ;
		UNPROTECT(1) ;
	}

} ;


} // namespace

#endif
