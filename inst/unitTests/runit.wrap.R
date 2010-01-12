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

if( Rcpp:::capabilities()[["initializer lists"]] ){ 

	test.wrap.initializerlist.int <- function(){
		funx <- cfunction(signature(), '
		return Rcpp::wrap( {0,1,2} ); 
		', Rcpp=TRUE, verbose=FALSE)
		checkEquals( funx(), 0:2, 
			msg = "wrap( initializer_list<int> )" )
	}
	
	test.wrap.initializerlist.double <- function(){
		funx <- cfunction(signature(), '
		return Rcpp::wrap( {0.0,1.0,2.0} ); 
		', Rcpp=TRUE, verbose=FALSE)
		checkEquals( funx(), as.numeric(0:2), 
			msg = "wrap( initializer_list<double> )" )
	}
	
	test.wrap.initializerlist.bool <- function(){
		funx <- cfunction(signature(), '
		return Rcpp::wrap( {false, true, false} ); 
		', Rcpp=TRUE, verbose=FALSE)
		checkEquals( funx(), c(FALSE, TRUE, FALSE), 
			msg = "wrap( initializer_list<bool> )" )
	}
	
	test.wrap.initializerlist.Rbyte <- function(){
		funx <- cfunction(signature(), '
		return Rcpp::wrap( { (Rbyte)0, (Rbyte)1 } ); 
		', Rcpp=TRUE, verbose=FALSE)
		checkEquals( funx(), as.raw(0:1), 
			msg = "wrap( initializer_list<Rbyte> )" )
	}
	
	# test.wrap.initializerlist.RObject <- function(){
	# 	funx <- cfunction(signature(), '
	# 	return Rcpp::wrap( { Rcpp::wrap(1), Rcpp::wrap("foo"), Rcpp::wrap(1.0)  } ); 
	# 	', Rcpp=TRUE, verbose=FALSE)
	# 	checkEquals( funx(), list(1L, "foo", 1.0) , 
	# 		msg = "wrap( initializer_list<SEXP> )" )
	# }

}
