// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
/* :tabSize=4:indentSize=4:noTabs=false:folding=explicit:collapseFolds=1: */
//
// wrap.h: Rcpp R/C++ interface class library -- wrap implementations
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

#ifndef Rcpp_internal_wrap_h
#define Rcpp_internal_wrap_h

// this is a private header, included in RcppCommon.h
// don't include it directly

namespace Rcpp{

// pre-declaring wrap :
template <typename T> SEXP wrap(const T& object) ;

namespace internal{


// {{{ information about R vectors
// welcome to template metaprogramming !!

// template that returns the SEXP type that is appropriate for 
// the type T, this is allways VECSXP (lists) unless it is specialized
template <typename T> struct r_sexptype{ enum{ rtype = VECSXP }; } ;
template<> struct r_sexptype<int>{ enum{ rtype = INTSXP } ; } ;
template<> struct r_sexptype<size_t>{ enum{ rtype = INTSXP } ; } ;
template<> struct r_sexptype<double>{ enum{ rtype = REALSXP } ; } ;
template<> struct r_sexptype<bool>{ enum{ rtype = LGLSXP } ; } ;
template<> struct r_sexptype<std::string>{ enum{ rtype = STRSXP } ; } ;
template<> struct r_sexptype<Rcomplex>{ enum{ rtype = CPLXSXP } ; } ;
template<> struct r_sexptype<Rbyte>{ enum{ rtype = RAWSXP } ; } ;

template<int RTYPE> struct storage_type{ typedef SEXP type ; } ;
template<> struct storage_type<INTSXP>{  typedef int type ; } ;
template<> struct storage_type<REALSXP>{ typedef double type ; } ;
template<> struct storage_type<CPLXSXP>{ typedef Rcomplex type ; } ;
template<> struct storage_type<RAWSXP>{ typedef Rbyte type ; } ;
template<> struct storage_type<LGLSXP>{ typedef int type ; } ;

template<int RTYPE, typename CTYPE> CTYPE* r_vector_start(SEXP x){ return static_cast<CTYPE*>(0) ; } ;
template<> int* r_vector_start<INTSXP,int>(SEXP x) ; 
template<> int* r_vector_start<LGLSXP,int>(SEXP x) ;
template<> double* r_vector_start<REALSXP,double>(SEXP x) ;
template<> Rbyte* r_vector_start<RAWSXP,Rbyte>(SEXP x) ; 
template<> Rcomplex* r_vector_start<CPLXSXP,Rcomplex>(SEXP x) ;                         

template <int RTYPE,typename CTYPE> CTYPE get_zero(){ return static_cast<CTYPE>(0) ; } ;
template<> Rcomplex get_zero<CPLXSXP,Rcomplex>() ;

// initializes a vector. The template does nothing because there is
template<int RTYPE> void r_init_vector(SEXP x){
	typedef storage_type<RTYPE> CTYPE ;
	CTYPE* start=r_vector_start<RTYPE>(x) ;
	std::fill( start, start + Rf_length(x), get_zero<RTYPE,CTYPE>(0) ) ;
}
// these are specifically overwritten so that they do nothing
// - character vectors : already initialized with ""
// - generic vectors   : already initialized with R_NilValue
// - expression vectors: already initialized with R_NilValue
template<> void r_init_vector<VECSXP>(SEXP x) ;
template<> void r_init_vector<EXPRSXP>(SEXP x) ;
template<> void r_init_vector<STRSXP>(SEXP x) ;
// }}}

// {{{ wrap_type_traits
struct wrap_type_stl_container_tag{};    // stl type container, with begin and end methods
struct wrap_type_primitive_tag{};        // primitive type
struct wrap_type_unknown_tag{};          // unknown, not sure what to do with this type

template <typename T> struct wrap_type_traits { typedef wrap_type_unknown_tag category; } ;

// partial specialization for stl containers
template <typename T> struct wrap_type_traits< std::vector<T> > { typedef wrap_type_stl_container_tag category ; } ;
template <typename T> struct wrap_type_traits< std::set<T> > { typedef wrap_type_stl_container_tag category ; } ;
template <typename T> struct wrap_type_traits< std::deque<T> > { typedef wrap_type_stl_container_tag category ; } ;
// #ifdef HAS_INIT_LISTS
// template <typename T> struct wrap_type_traits< std::initializer_list<T> > { typedef wrap_type_stl_container_tag category ; } ;
// #endif

template <> struct wrap_type_traits<int> { typedef wrap_type_primitive_tag category; } ;
template <> struct wrap_type_traits<double> { typedef wrap_type_primitive_tag category; } ;
template <> struct wrap_type_traits<Rbyte> { typedef wrap_type_primitive_tag category; } ;
template <> struct wrap_type_traits<Rcomplex> { typedef wrap_type_primitive_tag category; } ;
template <> struct wrap_type_traits<size_t> { typedef wrap_type_primitive_tag category; } ;
template <> struct wrap_type_traits<bool> { typedef wrap_type_primitive_tag category; } ;
template <> struct wrap_type_traits<std::string> { typedef wrap_type_primitive_tag category; } ;
template <> struct wrap_type_traits<char> { typedef wrap_type_primitive_tag category; } ;
// }}}

// {{{ r_type_traits
struct r_type_primitive_tag{} ;
struct r_type_string_tag{} ;
struct r_type_generic_tag{} ;
struct r_type_bool_tag{} ;

template <typename T> struct r_type_traits { typedef r_type_generic_tag category ; } ;
template<> struct r_type_traits<int>{ typedef r_type_primitive_tag category ; } ;
template<> struct r_type_traits<size_t>{ typedef r_type_primitive_tag category ; } ;
template<> struct r_type_traits<double>{ typedef r_type_primitive_tag category ; } ;
template<> struct r_type_traits<Rbyte>{ typedef r_type_primitive_tag category ; } ;
template<> struct r_type_traits<Rcomplex>{ typedef r_type_primitive_tag category ; } ;
template<> struct r_type_traits<bool>{ typedef r_type_bool_tag category ; } ;
template<> struct r_type_traits<std::string>{ typedef r_type_string_tag category ; } ;
template<> struct r_type_traits<char>{ typedef r_type_string_tag category ; } ;
// }}}

// {{{ range wrap 
template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, r_type_primitive_tag){ 
	size_t size = std::distance( first, last ) ;
	const int RTYPE = r_sexptype<T>::rtype ;
	SEXP x = PROTECT( Rf_allocVector( RTYPE, size ) );
	std::copy( first, last, r_vector_start<RTYPE, typename storage_type<RTYPE>::type >(x) ) ; 
	UNPROTECT(1) ;
	return x ;
} ;

template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, r_type_bool_tag){ 
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( LGLSXP, size ) );
	std::transform( first, last, LOGICAL(x), bool_to_Rboolean ) ; 
	UNPROTECT(1) ;
	return x ;
} ;


// this implementation is used when T is not a primitive type
// T needs to be wrappable though
template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, r_type_generic_tag ){ 
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( VECSXP, size ) );
	size_t i =0 ;
	while( i < size ){
		SET_VECTOR_ELT( x, i, ::Rcpp::wrap(*first) ) ;
		i++ ;
		++first ;
	}
	UNPROTECT(1) ;
	return x ;
} ;

template<typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, r_type_string_tag ){
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( STRSXP, size ) ) ;
	size_t i = 0 ;
	std::string buffer ;
	while( i < size ){
		buffer = *first ;
		SET_STRING_ELT( x, i, Rf_mkChar( buffer.c_str()) ) ;
		i++ ;
		++first ;
	}
	UNPROTECT(1) ;
	return x ;
}

template<typename InputIterator, typename T>
SEXP range_wrap_dispatch( InputIterator first, InputIterator last ){
	return range_wrap_dispatch___impl<InputIterator,T>( first, last, typename r_type_traits<T>::category() ) ;
}

// we use the iterator trait to make the dispatch
template <typename InputIterator>
SEXP range_wrap(InputIterator first, InputIterator last){
	return range_wrap_dispatch<InputIterator,typename std::iterator_traits<InputIterator>::value_type>( first, last ) ;
}
// }}}

// {{{ primitive wrap (wrapping a single primitive value)

// for 'easy' primitive types: int, double, Rbyte, Rcomplex
template <typename T>
SEXP primitive_wrap__impl( const T& object, r_type_primitive_tag ){
	const int RTYPE = r_sexptype<T>::rtype ;
	SEXP x = PROTECT( Rf_allocVector( RTYPE, 1 ) );
	r_vector_start<RTYPE, typename storage_type<RTYPE>::type >(x)[0] = object ;
	UNPROTECT(1);
	return x;
}

// for bool
template <typename T>
SEXP primitive_wrap__impl( const T& object, r_type_bool_tag){
	SEXP x = PROTECT( ::Rf_allocVector( LGLSXP, 1) );
	LOGICAL(x)[0] = static_cast<int>(object);
	UNPROTECT(1) ; /* x */
	return x;
}

// for std::string
template <typename T>
SEXP primitive_wrap__impl( const T& object, r_type_string_tag){
	SEXP x = PROTECT( ::Rf_allocVector( STRSXP, 1) ) ;
	std::string y = object ; /* give a chance to implicit conversion */
	SET_STRING_ELT( x, 0, Rf_mkChar(y.c_str()) ) ;
	UNPROTECT(1) ;
	return x; 
}

// T is a primitive type : int, bool, double, std::string, etc ...
// wrapping this in an R vector of the appropriate type
template <typename T>
SEXP primitive_wrap(const T& object){
	return primitive_wrap__impl( object, typename r_type_traits<T>::category() ) ;
}
// }}}

// {{{ wrap dispatch
// generic wrap for stl containers
template <typename T> SEXP wrap_dispatch( const T& object, wrap_type_stl_container_tag ){
	return range_wrap( object.begin(), object.end() ) ;
}
template <typename T> SEXP wrap_dispatch( const T& object, wrap_type_primitive_tag ){
	return primitive_wrap( object ) ;
}
template <typename T> SEXP wrap_dispatch( const T& object, wrap_type_unknown_tag ){
	SEXP x = object ;
	return x ;
}
// }}}

} // internal

template <typename T> SEXP wrap(const T& object){
	return internal::wrap_dispatch( object, typename internal::wrap_type_traits<T>::category() ) ;
}
// {{{ // explicit instanciations (not needed)
// template SEXP wrap<int>(const int& object) ;
// template SEXP wrap<double>(const double& object) ;
// template SEXP wrap<Rbyte>(const Rbyte& object) ;
// template SEXP wrap<Rcomplex>(const Rcomplex& object) ;
// template SEXP wrap<bool>(const bool& object) ;
// template SEXP wrap<std::string>(const std::string& object) ;
// template SEXP wrap< std::vector<int> >( const std::vector<int>& object ) ;
// template SEXP wrap< std::vector<double> >( const std::vector<double>& object ) ;
// template SEXP wrap< std::vector<Rbyte> >( const std::vector<Rbyte>& object ) ;
// template SEXP wrap< std::vector<Rcomplex> >( const std::vector<Rcomplex>& object ) ;
// template SEXP wrap< std::vector<bool> >( const std::vector<bool>& object ) ;
// 
// template SEXP wrap< std::set<int> >( const std::set<int>& object ) ;
// template SEXP wrap< std::set<double> >( const std::set<double>& object ) ;
// template SEXP wrap< std::set<Rbyte> >( const std::set<Rbyte>& object ) ;
// 
// template SEXP wrap< std::deque<int> >( const std::deque<int>& object ) ;
// template SEXP wrap< std::deque<double> >( const std::deque<double>& object ) ;
// template SEXP wrap< std::deque<Rbyte> >( const std::deque<Rbyte>& object ) ;
// template SEXP wrap< std::deque<Rcomplex> >( const std::deque<Rcomplex>& object ) ;
// template SEXP wrap< std::deque<bool> >( const std::deque<bool>& object ) ;
// }}}

// special cases - FIXME : these are not template specializations of wrap<>
inline SEXP wrap(const char* const v ){ return Rf_mkString(v) ; } ;

// wrap( { ... } ) : disabled for now
// #ifdef HAS_INIT_LISTS
// inline SEXP wrap(std::initializer_list<bool> v) { return internal::range_wrap( v.begin() , v.end() ); };
// inline SEXP wrap(std::initializer_list<std::string> v ) { return internal::range_wrap( v.begin() , v.end() ); };
// inline SEXP wrap(std::initializer_list<SEXP> v ) { return internal::range_wrap( v.begin() , v.end() ); };
// inline SEXP wrap(std::initializer_list<Rbyte> v) { return internal::range_wrap( v.begin() , v.end() ); };
// inline SEXP wrap(std::initializer_list<double> v) { return internal::range_wrap( v.begin() , v.end() ); } ; 
// inline SEXP wrap(std::initializer_list<int> v) { return internal::range_wrap( v.begin() , v.end() ); } ; 
// #endif


} // Rcpp

#endif
