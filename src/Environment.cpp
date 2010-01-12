// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Environment.cpp: Rcpp R/C++ interface class library -- Environments
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

#include <Rcpp/Environment.h>
#include <Rcpp/Evaluator.h>
#include <Rcpp/Symbol.h>
#include <Rcpp/Language.h>
#include <Rcpp/wrap.h>

namespace Rcpp {

/* this comes from JRI, where it was introduced to cope with cases
   where bindings are locked */
struct safeAssign_s {
    SEXP sym, val, rho;
};
static void safeAssign(void *data) {
    struct safeAssign_s *s = (struct safeAssign_s*) data;
    Rf_defineVar(s->sym, s->val, s->rho);
}

struct safeFindNamespace_s {
    SEXP sym, val ;
};
static void safeFindNamespace(void *data) {
    struct safeFindNamespace_s *s = (struct safeFindNamespace_s*) data;
    s->val = R_FindNamespace(s->sym);
}

    Environment::Environment( SEXP x = R_GlobalEnv) throw(not_compatible) : RObject::RObject(x){
    	if( ! Rf_isEnvironment(x) ) {
    		
    		/* not an environment, but maybe convertible to one using 
    		   as.environment, try that */
    		Evaluator evaluator( Rf_lang2(Symbol("as.environment"), x ) ) ;
    		evaluator.run() ;
    		if( evaluator.successfull() ){
    			setSEXP( evaluator.getResult().asSexp() ) ;
    		} else{
    			throw not_compatible( "cannot convert to environment"  ) ; 
    		}
    	}
    }

    Environment::Environment( const std::string& name) throw(no_such_env) : RObject(R_EmptyEnv){
    	/* similar to matchEnvir@envir.c */
    	if( name == ".GlobalEnv" ) {
    		setSEXP( R_GlobalEnv ) ;
    	} else if( name == "package:base" ){
    		setSEXP( R_BaseEnv ) ;
    	} else{
    		Evaluator evaluator( Rf_lang2(Symbol("as.environment"), Rf_mkString(name.c_str()) ) ) ;
    		evaluator.run() ;
    		if( evaluator.successfull() ){
    			setSEXP( evaluator.getResult().asSexp() ) ;
    		} else{
    			throw no_such_env(name) ; 
    		}
    	}
    }
    
    Environment::Environment(int pos) throw(no_such_env) : RObject(R_GlobalEnv){
    	Evaluator evaluator( Rf_lang2(Symbol("as.environment"), Rf_ScalarInteger(pos) ) ) ;
    	evaluator.run() ;
    	if( evaluator.successfull() ){
    		setSEXP( evaluator.getResult() ) ;
    	} else{
    		throw no_such_env(pos) ; 
    	}
    }
    
    Environment::Environment( const Environment& other ) throw() {
	setSEXP( other.asSexp() ) ; 
    }
    
    Environment& Environment::operator=(const Environment& other) throw() {
    	setSEXP( other.asSexp() ) ; 
	return *this ;
    }
    
    Environment::~Environment(){
	logTxt( "~Environment" ) ;
    }
	
    SEXP Environment::ls( bool all = true) const {
	if( is_user_database() ){
	    R_ObjectTable *tb = (R_ObjectTable*)
		R_ExternalPtrAddr(HASHTAB(m_sexp));
	    return tb->objects(tb) ;
	} else{
	    Rboolean get_all = all ? TRUE : FALSE ;
	    return R_lsInternal( m_sexp, get_all ) ;
	}
	return R_NilValue ;
    }
    
    SEXP Environment::get( const std::string& name) const {
    	SEXP res = Rf_findVarInFrame( m_sexp, Rf_install(name.c_str())  ) ;
    	
    	if( res == R_UnboundValue ) return R_NilValue ;
    	
    	/* We need to evaluate if it is a promise */
	if( TYPEOF(res) == PROMSXP){
    		res = Rf_eval( res, m_sexp ) ;
    	}
    	return res ;
    }
    
    bool Environment::exists( const std::string& name) const{
    	SEXP res = Rf_findVarInFrame( m_sexp, Rf_install(name.c_str())  ) ;
    	return res != R_UnboundValue ;
    }
    
    bool Environment::assign( const std::string& name, SEXP x = R_NilValue) const throw(binding_is_locked){
    	if( exists( name) && bindingIsLocked(name) ) throw binding_is_locked(name) ;
    	
    	/* borrowed from JRI, we cannot just use defineVar since it might 
    	   crash on locked bindings */
    	   
    	/* TODO: we need to modify R_ToplevelExec so that it does not print 
    	         the error message as it currently does*/
    	struct safeAssign_s s;
    	s.sym = Rf_install( name.c_str() ) ;
    	if( !s.sym || s.sym == R_NilValue ) return false ;
    	
    	s.rho = m_sexp ;
    	s.val = x ;
    	return static_cast<bool>( R_ToplevelExec(safeAssign, (void*) &s) );
    }
    
    bool Environment::remove( const std::string& name) throw(binding_is_locked){
    	    if( exists(name) ){
    	    	    if( bindingIsLocked(name) ){
    	    	    	    throw binding_is_locked(name) ;
    	    	    } else{
    	    	    	    /* unless we want to copy all of do_remove, 
    	    	    	       we have to go back to R to do this operation */
    	    	    	    SEXP call = Rf_lang2( 
    	    	    	    	    Rf_install( ".Internal" ), 
    	    	    	    	    Rf_lcons( Rf_install( "remove" ), 
    	    	    	    	    	    Rf_cons( Rf_mkString(name.c_str()), 
    	    	    	    	    	    	    Rf_cons( m_sexp, 
    	    	    	    	    	    	    	    Rf_cons( Rf_ScalarLogical( FALSE ), R_NilValue )
    	    	    	    	    	    	    	    ) 
    	    	    	    	    	    	    )
    	    	    	    	    	    )
    	    	    	    	    ) ;
    	    	    	    Rf_eval( call, R_GlobalEnv ) ;
    	    	    }
    	    } else{
    	    	throw no_such_binding(name) ;
    	    }
	    return true; // to make g++ -Wall happy
    }
    
    bool Environment::isLocked() const{
    	return R_EnvironmentIsLocked(m_sexp);
    }
    
    bool Environment::bindingIsActive(const std::string& name) const throw(no_such_binding) {
    	if( !exists( name) ) throw no_such_binding(name) ;
    	return R_BindingIsActive(Rf_install(name.c_str()), m_sexp) ;
    }
    
    bool Environment::bindingIsLocked(const std::string& name) const throw(no_such_binding) {
    	if( !exists( name) ) throw no_such_binding(name) ;
    	return R_BindingIsLocked(Rf_install(name.c_str()), m_sexp) ;
    }
    
    void Environment::lock( bool bindings = false ) {
    	R_LockEnvironment( m_sexp, bindings ? TRUE: FALSE ) ;
    }
    
    void Environment::lockBinding(const std::string& name) throw(no_such_binding) {
    	if( !exists( name) ) throw no_such_binding(name) ;
    	R_LockBinding( Rf_install( name.c_str() ), m_sexp ); 
    }
    
    void Environment::unlockBinding(const std::string& name) throw(no_such_binding) {
    	if( !exists( name) ) throw no_such_binding(name) ;
    	R_unLockBinding( Rf_install( name.c_str() ), m_sexp );
    }
    
    bool Environment::is_user_database() const {
    	return OBJECT(m_sexp) && Rf_inherits(m_sexp, "UserDefinedDatabase") ;
    }
    
    /* static */
    
    Environment Environment::global_env() throw() {
    	return Environment(R_GlobalEnv) ;
    }
    
    Environment Environment::empty_env() throw() {
    	return Environment(R_EmptyEnv) ;
    }
    
    Environment Environment::base_env() throw(){
    	return Environment(R_BaseEnv) ;
    }
    
    Environment Environment::base_namespace() throw() {
    	return Environment(R_BaseNamespace) ;
    }
    
    Environment Environment::namespace_env(const std::string& package) throw(no_such_namespace) {
    	struct safeFindNamespace_s s;
    	s.sym = Rf_mkString( package.c_str() ) ;
    	if( !s.sym || s.sym == R_NilValue || !R_ToplevelExec(safeFindNamespace, (void*) &s) ){
    		throw no_such_namespace(package) ;
    	}
    	return s.val ;
    }
    
    Environment Environment::parent() const throw() {
    	return Environment( ENCLOS(m_sexp) ) ; 
    }
    
    /* exceptions */
    
    Environment::no_such_binding::no_such_binding(const std::string& binding) :
    	message( "no such binding : '" + binding + "'" ) {}
    const char* Environment::no_such_binding::what() const throw(){
    	return message.c_str() ;
    }
    Environment::no_such_binding::~no_such_binding() throw() {}
    
    Environment::binding_is_locked::binding_is_locked(const std::string& binding) : 
    	message("binding is locked : '" + binding + "'" ) {}
    const char* Environment::binding_is_locked::what() const throw(){
    	return message.c_str() ;
    }
    Environment::binding_is_locked::~binding_is_locked() throw() {}
    
    Environment::no_such_namespace::no_such_namespace(const std::string& package) : 
    	message("no such namespace : '" + package + "'" ) {}
    const char* Environment::no_such_namespace::what() const throw(){
    	return message.c_str() ;
    }
    Environment::no_such_namespace::~no_such_namespace() throw() {}
    
    Environment::no_such_env::no_such_env(const std::string& name) : 
    	message("no environment called : '" + name + "'" ) {}
    Environment::no_such_env::no_such_env(int pos) : 
    	message("no environment in the given position" ) {}
    const char* Environment::no_such_env::what() const throw(){
    	return message.c_str() ;
    }
    Environment::no_such_env::~no_such_env() throw() {}
    
    
    
    Environment::Binding::Binding( Environment& env, const std::string& name): 
    	env(env), name(name){}
    
    bool Environment::Binding::active() const{
    	return env.bindingIsActive( name ) ; 
    }
    
    bool Environment::Binding::exists() const{
    	return env.exists( name ) ; 
    }
    
    bool Environment::Binding::locked() const{
    	return env.bindingIsLocked( name ) ; 
    }
    
    void Environment::Binding::lock() {
    	    env.lockBinding( name ) ;
    }
    
    void Environment::Binding::unlock() {
    	    env.unlockBinding( name ) ;
    }
    
    Environment::Binding& Environment::Binding::operator=( SEXP rhs ){
    	    env.assign( name, rhs ) ;
    	    return *this ;
    }
    
    Environment::Binding& Environment::Binding::operator=( const Binding& rhs){
    	    env.assign( name, rhs.env.get(rhs.name) ) ;
    	    return *this ;
    }
    
    Environment::Binding::operator SEXP() const{
    	return env.get( name );    
    }
    
    Environment::Binding::operator RObject() const{
    	return wrap( env.get( name ) );
    }
    
    const Environment::Binding Environment::operator[]( const std::string& name) const{
    	return Binding( const_cast<Environment&>(*this), name );
    }
    
    Environment::Binding Environment::operator[]( const std::string& name) {
    	return Binding( *this, name ) ;
    }
    
} // namespace Rcpp

