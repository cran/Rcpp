#!/usr/bin/r -t
#
# Copyright (C) 2009 - 2010	Romain Francois
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

test.evaluator.error <- function(){
	funx <- cfunction(signature(x = "expression"),  '
	Rcpp::Evaluator evaluator( x ) ;
	evaluator.run( Rcpp::Environment::global_env() ) ;
	return evaluator.getError() ;
	', Rcpp=TRUE, verbose=FALSE)
	
	err <- funx( expression(stop("error")) ) 
	checkTrue( all( "simpleError" %in% class(err ) ), msg = "error capture" )
}

test.evaluator.ok <- function(){
	funx <- cfunction(signature(x = "expression"),  '
	Rcpp::Evaluator evaluator( x ) ;
	evaluator.run( Rcpp::Environment::global_env() ) ;
	return evaluator.getResult() ;
	', Rcpp=TRUE, verbose=FALSE)
	
	x <- funx( expression( sample(1:10) ) ) 
	checkEquals( sort(x), 1:10, msg = "Evaluator running fine" )
}

