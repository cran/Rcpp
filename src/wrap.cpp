// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// wrap.cpp: Rcpp R/C++ interface class library -- general R object wrapper
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

#include <Rcpp.h>

#include <algorithm>

#include <Rcpp/grow.h>
#include <Rcpp/wrap.h>
#include <Rcpp/as.h>
#include <Rcpp/RObject.h>
#include <Rcpp/XPtr.h>
#include <Rcpp/Environment.h>
#include <Rcpp/Evaluator.h>
#include <Rcpp/Symbol.h>
#include <Rcpp/Language.h>
#include <Rcpp/Named.h>
#include <Rcpp/Pairlist.h>
#include <Rcpp/Function.h>
#include <Rcpp/IntegerVector.h>
#include <Rcpp/NumericVector.h>
#include <Rcpp/RawVector.h>
#include <Rcpp/LogicalVector.h>
#include <Rcpp/GenericVector.h>
#include <Rcpp/WeakReference.h>
#include <Rcpp/CharacterVector.h>
#include <Rcpp/ExpressionVector.h>
#include <Rcpp/ComplexVector.h>

namespace Rcpp{

RObject wrap(SEXP m_sexp=R_NilValue){
	switch( TYPEOF(m_sexp) ){
		case SYMSXP:
			return Symbol(m_sexp) ;
		case ENVSXP:
			return Environment(m_sexp);
		case LISTSXP:
		case DOTSXP: /* maybe change this later */
			return Pairlist(m_sexp);
		case CLOSXP:
		case SPECIALSXP: /* maybe change this later */
		case BUILTINSXP: /* maybe change this later */
			return Function(m_sexp);
		case LANGSXP:
			return Language(m_sexp);
		case LGLSXP:
			return LogicalVector(m_sexp);
		case INTSXP:
			return IntegerVector(m_sexp);
		case REALSXP:
			return NumericVector(m_sexp);
		case CPLXSXP:
			return ComplexVector(m_sexp);
		case STRSXP:
			return CharacterVector(m_sexp);
		case VECSXP:
			return List(m_sexp);
		case EXPRSXP:
			return ExpressionVector(m_sexp);
		case WEAKREFSXP:
			return WeakReference(m_sexp);
		case RAWSXP:
			return RawVector(m_sexp);
		default:
			break ;
	}
	/* for all other, just use RObject */
	return RObject(m_sexp) ;
}

LogicalVector wrap(const bool & v){
    logTxt("RObject from bool\n");
    LogicalVector o(Rf_ScalarLogical(v));
    return o ;
}

NumericVector wrap(const double & v){
    logTxt("RObject from double\n");
    NumericVector o(Rf_ScalarReal(v));
    return o ;
}

IntegerVector wrap(const int & v){
    logTxt("RObject from int\n");
    IntegerVector o(Rf_ScalarInteger(v));
    return o ;
}

RawVector wrap(const Rbyte & v){
    logTxt("RObject from raw\n");
    RawVector o(Rf_ScalarRaw(v));
    return o ;
}

LogicalVector wrap(const std::vector<bool> & v){
    logTxt("RObject from bool vector\n");
    size_t n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(LGLSXP, n) );
    copy( v.begin(), v.end(), LOGICAL(m_sexp) ) ;
    LogicalVector o(m_sexp) ;
    UNPROTECT(1) ; /* m_sexp now preserved by o */
    return o ;
}

IntegerVector wrap(const std::vector<int> & v){
    logTxt("RObject from int vector\n");
    size_t n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(INTSXP, n) );
    copy( v.begin(), v.end(), INTEGER(m_sexp) ) ;
    IntegerVector o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

NumericVector wrap(const std::vector<double> & v){
    logTxt("RObject from double vector\n");
    size_t n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(REALSXP, n) );
    copy( v.begin(), v.end(), REAL(m_sexp) ) ;
    NumericVector o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

RawVector wrap(const std::vector<Rbyte> & v){
    logTxt("RObject from vector<Rbyte> \n");
    size_t n = v.size();
    SEXP m_sexp = PROTECT(Rf_allocVector(RAWSXP, n));
    copy( v.begin(), v.end(), RAW(m_sexp) ) ;
    RawVector o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

/* sets */

IntegerVector wrap(const std::set<int> & v){
    logTxt("RObject from set<int>\n");
    size_t n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(INTSXP, n) );
    copy( v.begin(), v.end(), INTEGER(m_sexp) ) ;
    IntegerVector o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

NumericVector wrap(const std::set<double> & v){
    logTxt("RObject from set<double>\n");
    size_t n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(REALSXP, n) );
    copy( v.begin(), v.end(), REAL(m_sexp) ) ;
    NumericVector o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

RawVector wrap(const std::set<Rbyte> & v){
    logTxt("RObject from set<Rbyte> \n");
    size_t n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(RAWSXP, n) );
    copy( v.begin(), v.end(), RAW(m_sexp) ) ;
    RawVector o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

CharacterVector wrap(const std::set<std::string> & v){
    logTxt("RObject from set<string>\n");
    size_t n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(STRSXP, n) );
    size_t i=0;
    std::set<std::string>::iterator it = v.begin(); 
    while( i<n ){
    	SET_STRING_ELT(m_sexp, i, Rf_mkChar(it->c_str()));
    	i++ ;
    	it++; 
    }
    CharacterVector o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

} // namespace Rcpp
