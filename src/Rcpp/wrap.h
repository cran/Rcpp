// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// wrap.h: Rcpp R/C++ interface class library -- general R object wrapper
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

#ifndef Rcpp_wrap_h
#define Rcpp_wrap_h

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#endif

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <set>
#include <Rcpp/IntegerVector.h>
#include <Rcpp/RawVector.h>
#include <Rcpp/NumericVector.h>
#include <Rcpp/LogicalVector.h>
#include <Rcpp/GenericVector.h>
#include <Rcpp/CharacterVector.h>

namespace Rcpp{ 

// factories

RObject wrap(SEXP m_sexp) ;

LogicalVector wrap(const bool & v); 
NumericVector wrap(const double & v);
IntegerVector wrap(const int & v);
inline IntegerVector wrap(const size_t & v){ return wrap( (int)v ) ; } ;
RawVector wrap(const Rbyte & v);

IntegerVector wrap(const std::vector<int> & v);
NumericVector wrap(const std::vector<double> & v);
RawVector wrap(const std::vector<Rbyte> & v);
LogicalVector wrap(const std::vector<bool> & v);

IntegerVector wrap(const std::set<int> & v);
NumericVector wrap(const std::set<double> & v);
RawVector wrap(const std::set<Rbyte> & v);
CharacterVector wrap(const std::set<std::string> & v) ; 

inline CharacterVector wrap(const char* const v){ return CharacterVector(v); }
inline CharacterVector wrap(const std::string & v){ return CharacterVector(v); }
inline CharacterVector wrap(const std::vector<std::string> & v){ return CharacterVector(v); }


#ifdef HAS_INIT_LISTS
inline IntegerVector wrap( std::initializer_list<int> list) { return IntegerVector( list ) ;}
inline NumericVector wrap( std::initializer_list<double> list) { return NumericVector( list ); } 
inline LogicalVector wrap( std::initializer_list<bool> list) { return LogicalVector(list); }
inline RawVector wrap(std::initializer_list<Rbyte> list) { return RawVector(list) ; }
inline CharacterVector wrap(std::initializer_list<std::string> list ){ return CharacterVector(list) ; }
inline List wrap( std::initializer_list<RObject> list) { return List(list); }
#endif

} // namespace Rcpp

#endif
