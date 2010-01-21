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


