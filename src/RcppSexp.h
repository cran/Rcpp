// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppSexp.h: Rcpp R/C++ interface class library -- SEXP support
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

#ifndef RcppSexp_h
#define RcppSexp_h

#include <RcppCommon.h>
#include <set>

/**
 * This class is DEPRECATED, will be DEFUNCT in a next version and 
 * eventually removed from Rcpp. 
 *
 * The alternative is to use the Rcpp::RObject class and the set of functions
 * Rcpp::wrap. 
 */
class RcppSexp {
public:
	
    /**
     * wraps the SEXP into an Rcpp::RObject. The RObject does not 
     * automatically enforce protection of the SEXP, so if you want to 
     * preserve the SEXP from R garbage collection, you must call the 
     * protect member function
     */
    RcppSexp(SEXP m_sexp = R_NilValue) : object( m_sexp ) {
    	    DEPRECATED() ; 
    };
    
    ~RcppSexp() ;
	
    RcppSexp(const double & v) : object( Rcpp::wrap( v ) ) { DEPRECATED() ; } ;
    RcppSexp(const int & v): object( Rcpp::wrap(v) ){ DEPRECATED() ; } ;
    RcppSexp(const Rbyte & v) : object( Rcpp::wrap(v) ){ DEPRECATED() ;} ;
    RcppSexp(const std::string & v) : object( Rcpp::wrap(v) ){ DEPRECATED() ;} ;
    RcppSexp(const bool & v) : object( Rcpp::wrap(v) ){ DEPRECATED() ; };
    
    RcppSexp(const std::vector<int> & v): object( Rcpp::wrap(v) ){ DEPRECATED() ; };
    RcppSexp(const std::vector<double> & v): object( Rcpp::wrap(v) ){ DEPRECATED() ; };
    RcppSexp(const std::vector<std::string> & v): object( Rcpp::wrap(v) ){ DEPRECATED() ;};
    RcppSexp(const std::vector<Rbyte> & v): object( Rcpp::wrap(v) ){ DEPRECATED() ;};
    RcppSexp(const std::vector<bool> & v): object( Rcpp::wrap(v) ){ DEPRECATED() ;};
    
    RcppSexp(const std::set<int> & v): object( Rcpp::wrap(v) ){ DEPRECATED() ;};
    RcppSexp(const std::set<double> & v): object( Rcpp::wrap(v) ){ DEPRECATED() ; };
    RcppSexp(const std::set<std::string> & v): object( Rcpp::wrap(v) ){ DEPRECATED() ; };
    RcppSexp(const std::set<Rbyte> & v): object( Rcpp::wrap(v) ){ DEPRECATED() ; };
    
    
    /* we don't provide implicit converters because 
       of Item 5 in More Effective C++ */
       inline bool                     asBool() const  {return object.asBool() ; } ;
       inline double                   asDouble() const   { return object.asDouble() ; }
       inline int                      asInt() const { return object.asInt() ; }
       inline Rbyte                    asRaw() const { return object.asRaw() ; }
       inline std::string              asStdString() const { return object.asStdString() ; }
       inline std::vector<int>         asStdVectorInt() const { return object.asStdVectorInt() ; }
       inline std::vector<double>      asStdVectorDouble() const { return object.asStdVectorDouble(); }
       inline std::vector<std::string> asStdVectorString() const { return object.asStdVectorString(); }
       inline std::vector<Rbyte>       asStdVectorRaw() const { return object.asStdVectorRaw() ;}
       inline std::vector<bool>        asStdVectorBool() const { return object.asStdVectorBool() ; }
    
    
    	/**
	 * Calls the preserve method of the wrapped RObject, which 
	 * prevents the underlying SEXP from being garbage collected
	 */
	inline void protect(){ /* object.preserve() ; */ } 
	
	/**
	 * calls the release method of the RObject. the underlying SEXP
	 * becomes subject of garbage collection
	 */
	inline void release() { /* object.release() */ };
	
	/**
	 * implicit conversion to SEXP
	 */
	inline operator SEXP() const { return object.asSexp() ; }
	
	
	/* attributes */
	
	/**
	 * extracts the names of the attributes of the wrapped SEXP
	 */
	inline std::vector<std::string> attributeNames() const { return object.attributeNames() };
    
    	/**
    	 * Identifies if the SEXP has the given attribute
    	 */
    	 inline bool hasAttribute( const std::string& attr) const { return object.hasAttribute( attr ) }; 
    
	/**
	 * extract the given attribute
	 */
	inline SEXP attr( const std::string& name) const { return object.attr( name) ; } ;
    
	/**
	 * is this object NULL
	 */
	inline bool isNULL() const{ return object.isNULL() ; }
    
	/**
	 * The SEXP typeof, calls TYPEOF on the underlying SEXP
	 */
	 inline int sexp_type() const {  return object.sexp_type()  ; }
    
	/** 
	 * explicit conversion to SEXP
	 */
	inline SEXP asSexp() const {  return object.asSexp() ; }
	
protected:
	
	/**
	 * The RObject this wraps
	 */
	Rcpp::RObject object ;
	
	void DEPRECATED(){ Rf_warning( "The class RcppSexp is deprecated, and will eventually be removed, please consider using Rcpp::RObject instead" ) ; }
};

#endif
