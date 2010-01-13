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

test.CharacterVector <- function(){
	funx <- cfunction(signature(), '
	CharacterVector x(10) ;
	for( int i=0; i<10; i++) x[i] = "foo" ;
	return x ;', 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
	checkEquals( funx(), rep("foo",10L), msg = "CharacterVector" )
}

test.CharacterVector.STRSXP <- function(){
	funx <- cfunction(signature(vec = "character" ), '
	CharacterVector x(vec) ;
	std::string st = "" ; 
	for( int i=0; i<x.size(); i++) { 
		st += x[i] ;
	}
	return wrap( st ) ;', 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
	checkEquals( funx(letters), paste(letters,collapse="" ), 
		msg = "CharacterVector( STRSXP) " )
}

test.CharacterVector.initializer.list <- function(){
	if( Rcpp:::capabilities()[["initializer lists"]] ){
		funx <- cfunction(signature(), '
		CharacterVector x = {"foo", "bar"} ;
		return x ;', 
			Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
		checkEquals( funx(), c("foo","bar"), msg = "CharacterVector( initializer list) " )
	}
}

test.CharacterVector.plusequals <- function(){
	funx <- cfunction(signature(), '
	CharacterVector x(2) ;
	x[0] = "foo" ; 
	x[1] = "bar" ;
	x[0] += "bar" ;
	x[1] += x[0] ;
	return x ;
	', Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
	checkEquals( funx(), c("foobar", "barfoobar"), 
		msg = "StringProxy::operator+=" )
}

