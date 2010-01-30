// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// as.cpp: Rcpp R/C++ interface class library -- generic converters from SEXP
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

namespace Rcpp{ 

template<> double as<double>(SEXP m_sexp) {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("as<double> expects single value");
    }
    switch( TYPEOF(m_sexp) ){
    	case LGLSXP:
    		return Rboolean_to_double( LOGICAL(m_sexp)[0] ) ; 
    	case REALSXP:
    		return REAL(m_sexp)[0] ; 
    	case INTSXP:
    		return int_to_double( INTEGER(m_sexp)[0] ); 
    	case RAWSXP:
    		return static_cast<double>( RAW(m_sexp)[0] );
    	default:
    		throw std::range_error("as<double> invalid type");
    }
    return 0.0 ; 	// never reached
}

template<> int as<int>(SEXP m_sexp) {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("as<int> expects single value");
    }
    switch( TYPEOF(m_sexp)){
    	case LGLSXP:
    		return Rboolean_to_int( LOGICAL(m_sexp)[0] ) ; 
    	case REALSXP:
    		return double_to_int( REAL(m_sexp)[0] ); // some of this might be lost
    	case INTSXP:
    		return INTEGER(m_sexp)[0]; 
    	case RAWSXP:
    		return static_cast<int>( RAW(m_sexp)[0] );
    	default:
    		throw std::range_error("as<int>");
    }
    return 0; 	// never reached
}

template<> Rbyte as<Rbyte>(SEXP m_sexp) {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("as<Rbyte> expects single value");
    }
    switch( TYPEOF(m_sexp) ){
    	case LGLSXP:
    		return Rboolean_to_Rbyte( LOGICAL(m_sexp)[0] ) ; 
    	case REALSXP:
    		return double_to_Rbyte( REAL(m_sexp)[0] );
    	case INTSXP:
    		return int_to_Rbyte( INTEGER(m_sexp)[0] );
    	case RAWSXP:
    		return RAW(m_sexp)[0] ;
    	default:
    		throw std::range_error("as<Rbyte> expects raw, double or int");
    }
    return (Rbyte)0; 	// never reached
}

template<> bool as<bool>(SEXP m_sexp) {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("as<bool> expects single value");
    }
    switch( TYPEOF(m_sexp) ){
    	case LGLSXP:
    		return LOGICAL(m_sexp)[0] ? true : false ; 
    	case REALSXP:
    		return double_to_bool( REAL(m_sexp)[0] ) ;
    	case INTSXP:
    		return int_to_bool( INTEGER(m_sexp)[0] ) ;
    	case RAWSXP:
    		return Rbyte_to_bool( RAW(m_sexp)[0] );
    	default:
    		throw std::range_error("as<bool> expects raw, double or int");
    }
    return false; 	// never reached
}

template<> std::string as<std::string>(SEXP m_sexp) {
    if (Rf_length(m_sexp) != 1) {
    	    throw std::range_error("as<std::string> expects single value");
    }
    if (!Rf_isString(m_sexp)) {
    	    throw std::range_error("as<std::string> expects string");
    }
    return std::string(CHAR(STRING_ELT(m_sexp,0)));
}

template<> std::vector<bool> as< std::vector<bool> >(SEXP m_sexp) {
    int n = Rf_length(m_sexp);
    std::vector<bool> v(n);
    switch( TYPEOF(m_sexp) ){
    case LGLSXP:
    	transform( LOGICAL(m_sexp), LOGICAL(m_sexp)+n, v.begin(), Rboolean_to_bool ) ;
    	break ;
    case INTSXP:
    	transform( INTEGER(m_sexp), INTEGER(m_sexp)+n, v.begin(), int_to_bool ) ;
    	break;
    case REALSXP:
    	transform( REAL(m_sexp), REAL(m_sexp)+n, v.begin(), double_to_bool ) ;
    	break;
    case RAWSXP:
    	transform( RAW(m_sexp), RAW(m_sexp)+n, v.begin(), Rbyte_to_bool ) ;
    	break;
    default:
    		throw std::range_error( "as< vector<bool> >: invalid R type" ) ; 
    }
    return v;
}


template<> std::vector<int> as< std::vector<int> >(SEXP m_sexp){
    int n = Rf_length(m_sexp);
    std::vector<int> v(n);
    switch( TYPEOF(m_sexp) ){
    case INTSXP:
    	v.assign( INTEGER(m_sexp), INTEGER(m_sexp)+n ) ;
    	break;
    case LGLSXP:
    	transform( LOGICAL(m_sexp), LOGICAL(m_sexp)+n, v.begin(), Rboolean_to_int ) ;
    	break;
    case REALSXP:
    	transform( REAL(m_sexp), REAL(m_sexp)+n, v.begin(), double_to_int ) ;
    	break;
    case RAWSXP:
    	transform( RAW(m_sexp), RAW(m_sexp)+n, v.begin(), Rbyte_to_int ) ;
    	break;
    default:
    		throw std::range_error( "as< vector<int> >: invalid R type" ) ; 
    }
    return v;
}

template<> std::vector<Rbyte> as< std::vector<Rbyte> >(SEXP m_sexp) {
    int n = Rf_length(m_sexp);
    std::vector<Rbyte> v(n);
    switch( TYPEOF(m_sexp) ){
    case LGLSXP:
    	transform( LOGICAL(m_sexp), LOGICAL(m_sexp)+n, v.begin(), Rboolean_to_Rbyte ) ;
    	break ;
    case RAWSXP:
    	v.assign( RAW(m_sexp), RAW(m_sexp)+n ) ;
    	break ;
    case REALSXP:
    	transform( REAL(m_sexp), REAL(m_sexp)+n, v.begin(), double_to_Rbyte ) ;
    	break;
    case INTSXP:
    	transform( INTEGER(m_sexp), INTEGER(m_sexp)+n, v.begin(), int_to_Rbyte ) ;
    	break;
    default:
    	throw std::range_error("as< vector<Rbyte> > expects raw, double or int");
    }
    return v;
}

template<> std::vector<double> as< std::vector<double> >(SEXP m_sexp){
    int n = Rf_length(m_sexp);
    std::vector<double> v(n);
    switch( TYPEOF(m_sexp) ){
    case LGLSXP:
    	transform( LOGICAL(m_sexp), LOGICAL(m_sexp)+n, v.begin(), Rboolean_to_double ) ;
    	break ;
    case RAWSXP:
    	transform( RAW(m_sexp), RAW(m_sexp)+n, v.begin(), Rbyte_to_double ) ;
    	break ;
    case REALSXP:
    	v.assign( REAL(m_sexp), REAL(m_sexp)+n) ;
    	break;
    case INTSXP:
    	transform( INTEGER(m_sexp), INTEGER(m_sexp)+n, v.begin(), int_to_double) ;
    	break;
    default:
    	    throw std::range_error("as< vector<double> >:  expects raw, double or int");
    }
    return v;
}


template<> std::vector<std::string> as< std::vector<std::string> >(SEXP m_sexp){
    int n = Rf_length(m_sexp);
    std::vector<std::string> v(n);
    if (!Rf_isString(m_sexp)) {
    	    throw std::range_error("as< vector<string> >:  expects string");
    }
    for (int i = 0; i < n; i++) {
	v[i] = std::string(CHAR(STRING_ELT(m_sexp,i)));
    }
    return v;
}

} // namespace Rcpp

