// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppSexp.h: Rcpp R/C++ interface class library -- SEXP support
//
// Copyright (C) 2009 Dirk Eddelbuettel
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

#ifndef RcppSexp_h
#define RcppSexp_h

#include <RcppCommon.h>

class RcppSexp {
public:
    RcppSexp(SEXP sexp, int numprot=0) : m_sexp(sexp) { }
    RcppSexp() : m_sexp(R_NilValue) { }
    RcppSexp(const double & v);
    RcppSexp(const int & v);
    RcppSexp(const std::string & v);
    RcppSexp(const std::vector<int> & v);
    RcppSexp(const std::vector<double> & v);
    RcppSexp(const std::vector<std::string> & v);
    ~RcppSexp();

    double                   asDouble() const;
    int                      asInt() const;
    std::string              asStdString() const;
    std::vector<int>         asStdVectorInt() const;
    std::vector<double>      asStdVectorDouble() const;
    std::vector<std::string> asStdVectorString() const;
    SEXP                     asSexp() const;
    
private:
    SEXP m_sexp;
};

#endif
