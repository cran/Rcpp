#!/usr/bin/r -t
#
# Copyright (C) 2010	Dirk Eddelbuettel and Romain Francois
#
# This file is part of Rcpp.
#
# Rcpp is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Rcpp is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

.setUp <- function(){
	suppressMessages( require( inline ) )
}

test.List <- function(){
	funx <- cfunction(signature(), '
	List x(10) ;
	for( int i=0; i<10; i++) x[i] = Rf_ScalarInteger( i * 2)  ;
	return x ;', 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
	checkEquals( funx(), as.list( 2*0:9), msg = "GenericVector" )
}

test.List.template <- function(){
	funx <- cfunction(signature(), '
	List x(4) ;
	x[0] = "foo"  ;
	x[1] = 10 ;
	x[2] = 10.2 ;
	x[3] = false; 
	return x ;', 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
	checkEquals( funx(), 
		list( "foo", 10L, 10.2, FALSE), 
		msg = "GenericVector" )
}

test.List.VECSXP <- function(){
	funx <- cfunction(signature(vec = "list" ), '
	List x(vec) ;
	return x ;', 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
	checkEquals( funx(list(1,2)), list(1,2), msg = "GenericVector( VECSXP) " )
}

test.List.initializer.list <- function(){
	if( Rcpp:::capabilities()[["initializer lists"]] ){
		funx <- cfunction(signature(), '
		SEXP x0 = PROTECT( Rf_ScalarInteger( 0 ) ) ;
		SEXP x1 = PROTECT( Rf_ScalarInteger( 1 ) ) ;
		SEXP x2 = PROTECT( Rf_ScalarInteger( 2 ) ) ;
		List x = { x0, x1, x2} ;
		UNPROTECT(3) ;
		return x ;', 
			Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;", cxxargs="-std=c++0x" )
		checkEquals( funx(), as.list(0:2), msg = "List( initializer list) " )
	}
}

test.List.matrix.indexing <- function(){
	
	funx <- cfunction(signature(x = "character" ), '
		GenericVector m(x) ;
		GenericVector out(4) ;
		for( size_t i=0 ; i<4; i++){
			out[i] = m(i,i) ;
		}
		return out ;
	', Rcpp = TRUE, includes = "using namespace Rcpp;"  )
	
	# a matrix of integer vectors
	x <- structure( lapply( 1:16, function(x) seq.int(x) ), dim = c( 4, 4) )
	checkEquals( funx(x), diag(x), msg = "matrix indexing" )
	
	funx <- cfunction(signature(x = "integer" ), '
		GenericVector m(x) ;
		for( size_t i=0 ; i<4; i++){
			m(i,i) = "foo" ;
		}
		return m ;
	', Rcpp = TRUE, includes = "using namespace Rcpp;"  )
	checkEquals( diag(funx(x)), rep(list("foo"), 4) , 
		msg = "matrix indexing lhs" )
	
	# drop dimensions
	dim(x) <- NULL
	checkException( funx(x) , msg = "not a matrix" )
}

test.List.Dimension.constructor <- function(){

	funx <- cfunction(signature(), '
		return List( Dimension( 5 ) ) ;
	', Rcpp = TRUE, includes = "using namespace Rcpp;"  )
	checkEquals( funx(), 
		rep(list(NULL),5) , 
		msg = "List( Dimension(5))" )
	
	funx <- cfunction(signature(), '
		return List( Dimension( 5, 5 ) ) ;
	', Rcpp = TRUE, includes = "using namespace Rcpp;"  )
	checkEquals( funx(), 
		structure( rep( list(NULL), 25), dim = c(5,5) ),
		msg = "List( Dimension(5,5))" )
	
	funx <- cfunction(signature(), '
		return List( Dimension( 2, 3, 4) ) ;
	', Rcpp = TRUE, includes = "using namespace Rcpp;"  )
	checkEquals( funx(), 
		array( rep(list(NULL)), dim = c(2,3,4) ) , 
		msg = "List( Dimension(2,3,4))" )
}


