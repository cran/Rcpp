// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Environment.h: Rcpp R/C++ interface class library -- access R environments
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

#ifndef Rcpp_Environment_h
#define Rcpp_Environment_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>

namespace Rcpp{ 

class Environment: public RObject{
public:

   /**
     * Exception thrown when attempting to perform an operation on 
     * a binding and there is no such binding
     */
    class no_such_binding: public std::exception{
    	public:
    		/**
    		 * @param binding name of the binding
    		 */
    		no_such_binding( const std::string& binding) ;
    		
    		/**
    		 * The message: no such binding : '{binding}' 
    		 */
    		const char* what() const throw();
    		
    		~no_such_binding() throw() ;
    		
    	private:
    		std::string message ;
    } ;
    
    /**
     * Exception thrown when attempting to assign a value to a binding 
     * that is locked
     */
    class binding_is_locked: public std::exception{
    	public:
    		/**
    		 * @param binding name of the binding
    		 */
    		binding_is_locked( const std::string& binding) ;
    		
    		/**
    		 * The message: binding is locked : '{binding}' 
    		 */
    		const char* what() const throw() ;
    		
    		~binding_is_locked() throw() ;
    	private:
    		std::string message ;
    } ;
    
    /**
     * Exception thrown when attempting to get a namespace that does
     * not exist
     */
    class no_such_namespace: public std::exception{
    	public:
    		/**
    		 * @param package name of the package
    		 */
    		no_such_namespace( const std::string& package) ;
    		
    		/**
    		 * The message: no such namespace : '{package}' 
    		 */
    		const char* what() const throw() ;
    		
    		~no_such_namespace() throw() ;
    	private:
    		std::string message ;
    } ;
    
    
    /**
     * wraps the given environment
     *
     * if the SEXP is not an environment, and exception is thrown
     */
    Environment(SEXP m_sexp) ;
    
    /**
     * Nothing specific
     */ 
    ~Environment() ;
	
    /**
     * The list of objects in the environment
     * 
     * the same as calling this from R: 
     * > ls( envir = this, all = all )
     *
     * @param all same meaning as in ?ls
     */ 
    SEXP ls(bool all) const ;
    
    /**
     * Get an object from the environment
     *
     * @param name name of the object
     *
     * @return a SEXP (possibly R_NilValue)
     */
    SEXP get(const std::string& name) const ;
    
    /**
     * Indicates if an object called name exists in the 
     * environment
     *
     * @param name name of the object
     *
     * @return true if the object exists in the environment
     */
    bool exists( const std::string& name ) const ;
    
    /**
     * Attempts to assign x to name in this environment
     *
     * @param name name of the object to assign
     * @param x object to assign
     *
     * @return true if the assign was successfull
     * see ?bindingIsLocked
     *
     * @throw binding_is_locked if the binding is locked
     */
    bool assign( const std::string& name, SEXP x ) const throw(binding_is_locked) ;
    
    /**
     * @return true if this environment is locked
     * see ?environmentIsLocked for details of what this means
     */
    bool isLocked() const ;
    
    /**
     * locks this environment. See ?lockEnvironment
     *
     * @param bindings also lock the bindings of this environment ?
     */
    void lock(bool bindings) ;
    
    /* maybe we should have a separate class, e.g. Binding to deal
       with the 4 below functions ? */
    
    /**
     * Locks the given binding in the environment. 
     * see ?bindingIsLocked
     *
     * @throw no_such_binding if there is no such binding in this environment
     */
    void lockBinding(const std::string& name) throw(no_such_binding);
    
    /**
     * unlocks the given binding
     * see ?bindingIsLocked
     *
     * @throw no_such_binding if there is no such binding in this environment
     */
    void unlockBinding(const std::string& name) throw(no_such_binding) ;
    
    /**
     * @param name name of a potential binding
     *
     * @return true if the binding is locked in this environment
     * see ?bindingIsLocked
     *
     * @throw no_such_binding if there is no such binding in this environment
     */
    bool bindingIsLocked(const std::string& name) const throw(no_such_binding) ;
    
    /**
     *
     * @param name name of a binding
     * 
     * @return true if the binding is active in this environment
     * see ?bindingIsActive
     *
     * @throw no_such_binding if there is no such binding in this environment
     */
    bool bindingIsActive(const std::string& name) const throw(no_such_binding) ;
    
    /** 
     * Indicates if this is a user defined database.
     */
    bool is_user_database() const ;
    
    /**
     * @return the global environment. See ?globalenv
     */
    static Environment global_env() throw() ;
    
    /**
     * @return The empty environment. See ?emptyenv
     */
    static Environment empty_env() throw() ;
    
    /**
     * @return the base environment. See ?baseenv
     */
    static Environment base_env() throw() ;
    
    /**
     * @return the base namespace. See ?baseenv
     */
    static Environment base_namespace() throw() ;
    
    /**
     * @param name the name of the package of which we want the namespace
     *
     * @return the namespace of the package
     *
     * @throw no_such_namespace 
     */
    static Environment namespace_env(const std::string& ) throw(no_such_namespace) ;
};

} // namespace Rcpp

#endif
