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
	suppressMessages( require( datasets ) )
	data( iris )
}

test.RObject.asDouble <- function(){
	foo <- '
	double d = as<double>(x);
	return(wrap( 2*d ) );
	'
	funx <- cfunction(signature(x="numeric"), foo, 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
	checkEquals( funx(2.123), 4.246, msg = "RObject.asDouble()" )
	checkEquals( funx(2), 4, msg = "RObject.asDouble()" )
	checkException( funx(x='2'), msg = "RObject.asDouble() can not convert character" )
	checkException( funx(x=2:3), msg = "RObject.asDouble() expects the vector to be of length 1" )
	checkEquals( funx(2L), 4.0, msg = "RObject.asDouble()" )
}

test.RObject.asInt <- function(){
	foo <- '
	int i = as<int>(x) ;
	return(wrap( 2*i ) ); '
	funx <- cfunction(signature(x="numeric"), foo, 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
	checkEquals( funx(2.123), 4L, msg = "RObject.asInt()" )
	checkEquals( funx(2), 4L, msg = "RObject.asInt()" )
	checkEquals( funx(2L), 4.0, msg = "RObject.asInt()" )
	checkEquals( funx(as.raw(2L)), 4.0, msg = "RObject.asInt()" )
	checkException( funx(x='2'), msg = "RObject.asInt() can not convert character" )
	checkException( funx(x=2:3), msg = "RObject.asInt() expects the vector to be of length 1" )
	
}

test.RObject.asStdString <- function(){
	foo <- '
	std::string s = as<std::string>(x) ;
	return(wrap( s+s ) );'
	funx <- cfunction(signature(x="character"), foo, 
		Rcpp=TRUE, verbose=FALSE , includes = "using namespace Rcpp;")
	checkEquals( funx("abc"), "abcabc", msg = "RObject.asStdString()" )
	checkException( funx(NULL), msg = "RObject.asStdString expects string" )
	checkException( funx(0L), msg = "RObject.asStdString expects string" )
	checkException( funx(0.1), msg = "RObject.asStdString expects string" )
	checkException( funx(as.raw(0L)), msg = "RObject.asStdString expects string" )
	
	checkException( funx(letters), msg = "RObject.asStdString expects single string" )
	
}

test.RObject.asRaw <- function(){
	foo <- '
	Rbyte i = as<Rbyte>(x);
	return(wrap( (Rbyte)(2*i) ) ); '
	funx <- cfunction(signature(x="raw"), foo, 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
	checkEquals( funx(1L), as.raw(2L), msg = "RObject.asRaw(integer)" )
	checkEquals( funx(1.3), as.raw(2L), msg = "RObject.asRaw(numeric)" )
	checkEquals( funx(as.raw(1)), as.raw(2L), msg = "RObject.asRaw(raw)" )
	checkException( funx(NULL) , msg = "RObject.asRaw(NULL) -> exception" )
	checkException( funx("foo") , msg = "RObject.asRaw(character) -> exception" )
	checkException( funx(1:2), msg = "RObject.asRaw(>1 integer) -> exception" )
	checkException( funx(as.numeric(1:2)), msg = "RObject.asRaw(>1 numeric) -> exception" )
	checkException( funx(as.raw(1:3)), msg = "RObject.asRaw(>1 raw) -> exception" )
	checkException( funx(integer(0)), msg = "RObject.asRaw(0 integer) -> exception" )
	checkException( funx(numeric(0)), msg = "RObject.asRaw(0 numeric) -> exception" )
	checkException( funx(raw(0)), msg = "RObject.asRaw(0 raw) -> exception" )
}

test.RObject.asLogical <- function(){
	foo <- '
	bool b = as<bool>(x);
	return(wrap( !b ));'
	funx <- cfunction(signature(x="logical"), foo, 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
	checkTrue( !funx(TRUE), msg = "RObject::asBool(TRUE) -> true" )
	checkTrue( funx(FALSE), msg = "RObject::asBool(FALSE) -> false" )
	checkTrue( !funx(1L), msg = "RObject::asBool(1L) -> true" )
	checkTrue( funx(0L), msg = "RObject::asBool(0L) -> false" )
	checkTrue( !funx(1.0), msg = "RObject::asBool(1.0) -> true" )
	checkTrue( funx(0.0), msg = "RObject::asBool(0.0) -> false" )
	checkTrue( !funx(as.raw(1)), msg = "RObject::asBool(aw.raw(1)) -> true" )
	checkTrue( funx(as.raw(0)), msg = "RObject::asBool(as.raw(0)) -> false" )
	
	checkException( funx(NULL), msg = "RObject::asBool(NULL) -> exception" )
	checkException( funx(c(TRUE,FALSE)), msg = "RObject::asBool(>1 logical) -> exception" )
	checkException( funx(1:2), msg = "RObject::asBool(>1 integer) -> exception" )
	checkException( funx(1:2+.1), msg = "RObject::asBool(>1 numeric) -> exception" )
	checkException( funx(as.raw(1:2)), msg = "RObject::asBool(>1 raw) -> exception" )
	
	checkException( funx(integer(0)), msg = "RObject.asBool(0 integer) -> exception" )
	checkException( funx(numeric(0)), msg = "RObject.asBool(0 numeric) -> exception" )
	checkException( funx(raw(0)), msg = "RObject.asBool(0 raw) -> exception" )
}

test.RObject.asStdVectorIntResultsSet <- function(){
	foo <- '
		std::vector<int> iv = RObject(x).asStdVectorInt();
		for (size_t i=0; i<iv.size(); i++) {
    	    iv[i] = 2*iv[i];
    	}
    	RcppResultSet rs;
    	rs.add("", iv);
    	return(rs.getSEXP());'
    funx <- cfunction(signature(x="numeric"), foo, 
    	Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
	checkEquals( funx(x=2:5), 2:5*2L, msg = "RObject(integer).asStdVectorInt via RcppResultSet" )
    checkEquals( funx(x=2:5+.1), 2:5*2L, msg = "RObject(numeric).asStdVectorInt via RcppResultSet" )
    checkEquals( funx(x=as.raw(2:5)), 2:5*2L, msg = "RObject(raw).asStdVectorInt via RcppResultSet" )
    checkException( funx("foo"), msg = "RObject(character).asStdVectorInt -> exception" )
    
}

test.RObject.asStdVectorInt <- function(){
    foo <- '
    std::vector<int> iv = RObject(x).asStdVectorInt();
    for (size_t i=0; i<iv.size(); i++) {
        iv[i] = 2*iv[i];
    }
    return(Rcpp::wrap( iv ) );'
    funx <- cfunction(signature(x="numeric"), foo, 
	Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
    checkEquals( funx(x=2:5), 2:5*2L, msg = "RObject(integer).asStdVectorInt" )
    checkEquals( funx(x=2:5+.1), 2:5*2L, msg = "RObject(numeric).asStdVectorInt" )
    checkEquals( funx(x=as.raw(2:5)), 2:5*2L, msg = "RObject(raw).asStdVectorInt" )
    checkException( funx("foo"), msg = "RObject(character).asStdVectorInt -> exception" )
    checkException( funx(NULL), msg = "RObject(NULL).asStdVectorInt -> exception" )
    
}

test.RObject.asStdVectorDouble <- function(){
	foo <- '
		std::vector<double> iv = RObject(x).asStdVectorDouble();
		for (size_t i=0; i<iv.size(); i++) {
	        iv[i] = 2*iv[i];
	    }
	 	return(Rcpp::wrap( iv ));'
	funx <- cfunction(signature(x="numeric"), foo,
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
	checkEquals( funx(x=0.1+2:5), 2*(0.1+2:5), msg = "RObject(numeric).asStdVectorDouble" )
	checkEquals( funx(x=2:5), 2*(2:5), msg = "RObject(integer).asStdVectorDouble" )
	checkEquals( funx(x=as.raw(2:5)), 2*(2:5), msg = "RObject(raw).asStdVectorDouble" )
	checkException( funx("foo"), msg = "RObject(character).asStdVectorDouble -> exception" )
    checkException( funx(NULL), msg = "RObject(NULL).asStdVectorDouble -> exception" )
    
}

test.RObject.asStdVectorRaw <- function(){
	foo <- '
    	std::vector<Rbyte> iv = RObject(x).asStdVectorRaw();
		for (size_t i=0; i<iv.size(); i++) {
    	    iv[i] = 2*iv[i];
    	}
 		return(Rcpp::wrap( iv ));'
	funx <- cfunction(signature(x="raw"), foo, 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
	checkEquals( funx(x=as.raw(0:9)), as.raw(2*(0:9)), msg = "RObject(raw).asStdVectorRaw" )
	checkEquals( funx(x=0:9), as.raw(2*(0:9)), msg = "RObject(integer).asStdVectorRaw" )
	checkEquals( funx(x=as.numeric(0:9)), as.raw(2*(0:9)), msg = "RObject(numeric).asStdVectorRaw" )
	checkException( funx("foo"), msg = "RObject(character).asStdVectorRaw -> exception" )
    checkException( funx(NULL), msg = "RObject(NULL).asStdVectorRaw -> exception" )
    
}

test.RObject.asStdVectorBool <- function(){
	foo <- '
		std::vector<bool> bv = RObject(x).asStdVectorBool();
		for (size_t i=0; i<bv.size(); i++) {
		    bv[i].flip() ;
		}
		return(Rcpp::wrap( bv ));'
	funx <- cfunction(signature(x="logical"), foo,
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
	checkEquals( funx(x=c(TRUE,FALSE)), c(FALSE, TRUE), msg = "RObject(logical).asStdVectorBool" )
	checkEquals( funx(x=c(1L, 0L)), c(FALSE, TRUE), msg = "RObject(integer).asStdVectorBool" )
	checkEquals( funx(x=c(1.0, 0.0)), c(FALSE, TRUE), msg = "RObject(numeric).asStdVectorBool" )
	checkEquals( funx(x=as.raw(c(1,0))), c(FALSE, TRUE), msg = "RObject(raw).asStdVectorBool" )
	checkException( funx("foo"), msg = "RObject(character).asStdVectorBool -> exception" )
    checkException( funx(NULL), msg = "RObject(NULL).asStdVectorBool -> exception" )
}

test.RObject.asStdVectorString <- function(){
	foo <- '
    	std::vector<std::string> iv = RObject(x).asStdVectorString();
		for (size_t i=0; i<iv.size(); i++) {
    	    iv[i] = iv[i] + iv[i];
    	}
 	return(Rcpp::wrap( iv ));'
	funx <- cfunction(signature(x="character"), foo, 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
	checkEquals( funx(c("foo", "bar")), c("foofoo", "barbar"), msg = "RObject(character).asStdVectorString" )
	checkException( funx(1L), msg = "RObject(integer).asStdVectorString -> exception" )
	checkException( funx(1.0), msg = "RObject(numeric).asStdVectorString -> exception" )
	checkException( funx(as.raw(1)), msg = "RObject(raw).asStdVectorString -> exception" )
	checkException( funx(TRUE), msg = "RObject(logical).asStdVectorString -> exception" )
	checkException( funx(NULL), msg = "RObject(NULL).asStdVectorString -> exception" )
}

test.RObject.stdsetint <- function(){
	foo <- '
		std::set<int> iv ;
		iv.insert( 0 ) ;
		iv.insert( 1 ) ;
		iv.insert( 0 ) ;
		return Rcpp::wrap( iv );'
	funx <- cfunction(signature(), foo, Rcpp=TRUE, verbose=FALSE, includes = "#include <set>" )
	checkEquals( funx(), c(0L, 1L), msg = "RObject( set<int> )" )
}

test.RObject.stdsetdouble <- function(){
	foo <- '
		std::set<double> ds;
		ds.insert( 0.0 );
		ds.insert( 1.0 );
		ds.insert( 0.0 );
		return(Rcpp::wrap( ds )); '
	funx <- cfunction(signature(), foo, Rcpp=TRUE, verbose=FALSE, includes = "#include <set>")
	checkEquals( funx(), as.numeric(0:1), msg = "RObject( set<double>" )
}
	
test.RObject.stdsetraw <- function(){
	foo <- '
		std::set<Rbyte> bs ;
		bs.insert( (Rbyte)0 ) ;
		bs.insert( (Rbyte)1 ) ;
		bs.insert( (Rbyte)0 ) ;
		return(Rcpp::wrap( bs )); '
	funx <- cfunction(signature(), foo, Rcpp=TRUE, verbose=FALSE, includes = "#include <set>")
	checkEquals( funx(), as.raw(0:1), msg = "RObject(set<raw>)" )
}

test.RObject.stdsetstring <- function(){
	foo <- '
		std::set<std::string> ss ;
		ss.insert( "foo" ) ;
		ss.insert( "bar" ) ;
		ss.insert( "foo" ) ;
		return(Rcpp::wrap( ss )); '
	funx <- cfunction(signature(), foo, Rcpp=TRUE, verbose=FALSE, include = "#include <set>" )
	checkEquals( funx(), c("bar", "foo"), msg = "RObject(set<string>)" )
}

test.RObject.attributeNames <- function(){
	funx <- cfunction(signature(x="data.frame"), '
		std::vector<std::string> iv = RObject(x).attributeNames();
		return(wrap( iv ));', 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
	checkTrue( all( c("names","row.names","class") %in% funx(iris)), msg = "RObject.attributeNames" )
}

test.RObject.hasAttribute <- function(){
	funx <- cfunction(signature(x="data.frame"), '
		bool has_class = RObject(x).hasAttribute( "class" ) ;
		return wrap( has_class ) ;', 
		Rcpp=TRUE, verbose=FALSE, 
		includes = "using namespace Rcpp;")
	checkTrue( funx( iris ), msg = "RObject.hasAttribute" )
}

test.RObject.attr <- function(){
	funx <- cfunction(signature(x="data.frame"), '
		return RObject(x).attr( "row.names" ) ;
		', Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
	checkEquals( funx( iris ), 1:150, msg = "RObject.attr" )
}

test.RObject.attr.set <- function(){
	funx <- cfunction(signature(), '
	RObject y = wrap("blabla") ;
	y.attr("foo") = 10 ;
	return y ; ', Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;")
	checkEquals( attr(funx(), "foo"), 10L, msg = "RObject.attr() = " )
}

test.RObject.isNULL <- function(){
	funx <- cfunction(signature(x="ANY"), '
		bool is_null = RObject(x).isNULL() ;
		return wrap( is_null ) ;
		', Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
	checkTrue( !funx( iris ), msg = "RObject.isNULL(iris) -> false" )
	checkTrue( funx(NULL), msg = "RObject.isNULL(NULL) -> true" )
	checkTrue( !funx(1L), msg = "RObject.isNULL(integer) -> false" )
	checkTrue( !funx(1.0), msg = "RObject.isNULL(numeric) -> false" )
	checkTrue( !funx(as.raw(1)), msg = "RObject.isNULL(raw) -> false" )
	checkTrue( !funx(letters), msg = "RObject.isNULL(character) -> false")
	checkTrue( !funx(funx), msg = "RObject.isNULL(function) -> false" )
	checkTrue( !funx(.GlobalEnv), msg = "RObject.isNULL(environment) -> false" )
}

