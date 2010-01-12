// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Evaluator.h: Rcpp R/C++ interface class library -- protected evaluation
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

#ifndef Rcpp_Evaluator_h
#define Rcpp_Evaluator_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/Environment.h>
#include <Rcpp/wrap.h>

namespace Rcpp{ 

class Evaluator{
public:
    Evaluator(SEXP expression ) ;
    ~Evaluator() ;
    void run(SEXP env) throw() ;
    void run() throw() ;
    inline RObject getResult() const { return result ; }
    inline RObject getError() const { return error ; }
    inline bool successfull() const { return !error_occured ; }
    
private:		
    SEXP expression ;
    bool error_occured ;
    RObject result ;
    RObject error ;
};

} // namespace Rcpp

#endif
