// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppParams.h: Rcpp R/C++ interface class library -- Parameters from R
//
// Copyright (C) 2005 - 2006 Dominick Samperi
// Copyright (C) 2008 - 2009 Dirk Eddelbuettel
//
// This library is free software; you can redistribute it and/or modify it 
// under the terms of the GNU Lesser General Public License as published by 
// the Free Software Foundation; either version 2.1 of the License, or (at 
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but 
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License 
// along with this library; if not, write to the Free Software Foundation, 
// Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 

#ifndef RcppParams_h
#define RcppParams_h

#include <RcppCommon.h>
#include <RcppDate.h>
#include <RcppDatetime.h>

class RcppParams {
public:
    RcppParams(SEXP params);
    void   		 checkNames(char *inputNames[], int len);
    bool		 exists(std::string name);
    double 		 getDoubleValue(std::string name);
    int    		 getIntValue(std::string name);
    std::string  getStringValue(std::string name);
    bool   		 getBoolValue(std::string name);
    RcppDate 	 getDateValue(std::string name);
    RcppDatetime getDatetimeValue(std::string name);

private:
    std::map<std::string, int> pmap;
    SEXP _params;
};

#endif
