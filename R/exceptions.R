# Copyright (C)        2009 Romain Francois
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

cpp_exception <- function( message = "C++ exception", class = NULL ){
	callstack <- sys.calls()
	ncalls <- length(callstack)
	call <- if( ncalls > 1L) callstack[[ ncalls - 1L ]] else match.call()
	classes <- c( class, "C++Error", "error", "condition" )
	condition <- structure( 
		list( message = message, call = call ), 
		class = classes )
	stop( condition )
}

# used by Rcpp::Evaluator
exceptions <- new.env()
setCurrentError <- function( condition = NULL) exceptions[["current"]] <- condition
resetCurrentError <- function() {
	setCurrentError(NULL)
	setErrorOccured(FALSE)
}
getCurrentError <- function() exceptions[["current"]]
errorOccured <- function() isTRUE( exceptions[["error_occured"]] )
setErrorOccured <- function(error_occured = TRUE) exceptions[["error_occured"]] <- error_occured
resetCurrentError()
protectedEval <- function(expr, env ){
	resetCurrentError()
	tryCatch( eval( expr, envir = env), error = function(e){
		setErrorOccured( TRUE )
		setCurrentError( e )
		invisible( NULL )
	} )
}
setErrorOccured(FALSE)

