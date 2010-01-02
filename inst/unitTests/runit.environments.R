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

test.environment.ls <- function(){
	funx <- cfunction(signature(x="environment"), '
	Rcpp::Environment env(x) ; 
	return env.ls(true) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env( )
	e$a <- 1:10
	e$b <- "foo"
	e$.c <- "hidden"
	checkEquals( funx(e), c("a","b", ".c"), msg = "Environment::ls(true)" )
	checkEquals( funx(asNamespace("Rcpp")), ls(envir=asNamespace("Rcpp"), all = TRUE), 
		msg = "Environment(namespace)::ls()" )
	
	funx <- cfunction(signature(x="environment"), '
	Rcpp::Environment env(x) ; 
	return env.ls(false) ;
	', Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx(e), c("a","b"), msg = "Environment::ls(false)" )
	checkEquals( funx(asNamespace("Rcpp")), ls(envir=asNamespace("Rcpp"), all = FALSE), 
		msg = "Environment(namespace)::ls()" )
	
}

test.environment.get <- function(){
	funx <- cfunction(signature(x="environment", name = "character" ), '
	Rcpp::Environment env(x) ;
	return env.get( Rcpp::wrap(name).asStdString() ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env( )
	e$a <- 1:10
	e$b <- "foo"
	
	checkEquals( funx( e, "a" ), e$a, msg = "Environment::get()" )
	checkEquals( funx( e, "foobar" ), NULL, msg = "Environment::get()" )
	checkEquals( funx( asNamespace("Rcpp"), "CxxFlags"), Rcpp:::CxxFlags, 
		msg = "Environment(namespace)::get() " )
	
}

test.environment.exists <- function(){
	funx <- cfunction(signature(x="environment", name = "character" ), '
	Rcpp::Environment env(x) ;
	std::string st = Rcpp::wrap(name).asStdString() ;
	return Rcpp::wrap( env.exists(st) ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env( )
	e$a <- 1:10
	e$b <- "foo"
	
	checkTrue( funx( e, "a" ), msg = "Environment::get()" )
	checkTrue( !funx( e, "foobar" ), msg = "Environment::get()" )
	checkTrue( funx( asNamespace("Rcpp"), "CxxFlags"), 
		msg = "Environment(namespace)::get() " )
}

test.environment.assign <- function(){
	
	funx <- cfunction(signature(x="environment", name = "character", object = "ANY" ), '
	Rcpp::Environment env(x) ;
	std::string st = Rcpp::wrap(name).asStdString() ;
	return Rcpp::wrap( env.assign(st, object) ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env( )
	checkTrue( funx(e, "a", 1:10 ), msg = "Environment::assign" )
	checkTrue( funx(e, "b", Rcpp:::CxxFlags ), msg = "Environment::assign" )
	checkEquals( ls(e), c("a", "b"), msg = "Environment::assign, checking names" )
	checkEquals( e$a, 1:10, msg = "Environment::assign, checking value 1" )
	checkEquals( e$b, Rcpp:::CxxFlags, msg = "Environment::assign, checking value 2" )
	
	lockBinding( "a", e )
	checkTrue( 
		tryCatch( { funx(e, "a", letters ) ; FALSE}, "Rcpp::Environment::binding_is_locked" = function(e) TRUE ), 
		msg = "cannot assign to locked binding (catch exception)" )

}

test.environment.isLocked <- function(){
	funx <- cfunction(signature(x="environment" ), '
	Rcpp::Environment env(x) ;
	return Rcpp::wrap( env.isLocked() ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env()
	checkTrue( !funx(e), msg = "Environment::isLocked( new.env) -> false" )
	checkTrue( funx(asNamespace("Rcpp")), msg = "Environment::isLocked( namespace:Rcpp ) -> true" )
	
}

test.environment.bindingIsActive <- function(){
	
	funx <- cfunction(signature(x="environment", name = "character" ), '
	Rcpp::Environment env(x) ;
	std::string st = Rcpp::wrap(name).asStdString() ;
	return Rcpp::wrap( env.bindingIsActive(st) ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env()
	e$a <- 1:10
	makeActiveBinding( "b", function(x) 10, e ) 

	checkTrue( !funx(e, "a" ), msg = "Environment::bindingIsActive( non active ) -> false" )
	checkTrue( funx(e, "b" ), msg = "Environment::bindingIsActive( active ) -> true" )
	checkTrue( 
		tryCatch( { funx(e, "xx" ) ; FALSE}, "Rcpp::Environment::no_such_binding" = function(e) TRUE ), 
		msg = "Environment::bindingIsActive(no binding) -> exception)" )
	
}

test.environment.bindingIsLocked <- function(){
	
	funx <- cfunction(signature(x="environment", name = "character" ), '
	Rcpp::Environment env(x) ;
	std::string st = Rcpp::wrap(name).asStdString() ;
	return Rcpp::wrap( env.bindingIsLocked(st) ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env()
	e$a <- 1:10
	e$b <- letters
	lockBinding( "b", e )
	
	checkTrue( !funx(e, "a" ), msg = "Environment::bindingIsActive( non active ) -> false" )
	checkTrue( funx(e, "b" ), msg = "Environment::bindingIsActive( active ) -> true" )
	checkTrue( 
		tryCatch( { funx(e, "xx" ) ; FALSE}, "Rcpp::Environment::no_such_binding" = function(e) TRUE ), 
		msg = "Environment::bindingIsLocked(no binding) -> exception)" )
	
}

test.environment.NotAnEnvironment <- function(){
	funx <- cfunction(signature(x="ANY"), 'Rcpp::Environment env(x) ;', 
		Rcpp=TRUE, verbose=FALSE)
	checkException( funx( funx ), msg = "not an environment" )
	checkException( funx( letters ), msg = "not an environment" )
	checkException( funx( iris ), msg = "not an environment" )
	checkException( funx( NULL ), msg = "not an environment" )
}


test.environment.lockBinding <- function(){
	funx <- cfunction(signature(x="environment", name = "character" ), '
	Rcpp::Environment env(x) ;
	std::string st = Rcpp::wrap(name).asStdString() ;
	env.lockBinding( st ) ;
	return R_NilValue ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env()
	e$a <- 1:10
	e$b <- letters
	funx(e, "b")
	checkTrue( bindingIsLocked("b", e ), msg = "Environment::lockBinding()" )
	checkTrue( 
		tryCatch( { funx(e, "xx" ) ; FALSE}, "Rcpp::Environment::no_such_binding" = function(e) TRUE ), 
		msg = "Environment::lockBinding(no binding) -> exception)" )
	
}

test.environment.unlockBinding <- function(){
	funx <- cfunction(signature(x="environment", name = "character" ), '
	Rcpp::Environment env(x) ;
	std::string st = Rcpp::wrap(name).asStdString() ;
	env.unlockBinding( st ) ;
	return R_NilValue ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env()
	e$a <- 1:10
	e$b <- letters
	lockBinding( "b", e )
	funx(e, "b")
	checkTrue( !bindingIsLocked("b", e ), msg = "Environment::lockBinding()" )
	checkTrue( 
		tryCatch( { funx(e, "xx" ) ; FALSE}, "Rcpp::Environment::no_such_binding" = function(e) TRUE ), 
		msg = "Environment::unlockBinding(no binding) -> exception)" )
	
}

test.environment.global.env <- function(){
	funx <- cfunction(signature(), 
	'return Rcpp::Environment::global_env(); ', Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx(), globalenv(), msg = "REnvironment::global_env" )
}

test.environment.empty.env <- function(){
	funx <- cfunction(signature(), 
	'return Rcpp::Environment::empty_env(); ', Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx(), emptyenv(), msg = "REnvironment::empty_env" )
}

test.environment.base.env <- function(){
	funx <- cfunction(signature(), 
	'return Rcpp::Environment::base_env(); ', Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx(), baseenv(), msg = "REnvironment::base_env" )
}

test.environment.empty.env <- function(){
	funx <- cfunction(signature(), 
	'return Rcpp::Environment::base_namespace(); ', Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx(), .BaseNamespaceEnv, msg = "REnvironment::base_namespace" )
}

test.environment.namespace.env <- function(){
	funx <- cfunction(signature(env = "character" ),  '
	std::string st = Rcpp::wrap(env).asStdString() ;
	return Rcpp::Environment::namespace_env(st); ', Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx("Rcpp"), asNamespace("Rcpp"), msg = "REnvironment::base_namespace" )
	checkTrue( 
		tryCatch( { funx("----" ) ; FALSE}, "Rcpp::Environment::no_such_namespace" = function(e) TRUE ), 
		msg = "Environment::namespace_env(no namespace) -> exception)" )
	
}



