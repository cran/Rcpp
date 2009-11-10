// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppList.h: Rcpp R/C++ interface class library -- 'list' type support
//
// Copyright (C) 2009 Dirk Eddelbuettel
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

#ifndef RcppList_h
#define RcppList_h

#include <RcppCommon.h>

// This class was first used in the RProtoBuf project (currently on r-forge only)
// but is more generally useful and hence moved over here
class RcppList {
public:
    RcppList(void);
    ~RcppList();
    void setSize(int size);
    void append(std::string name, double value);
    void append(std::string name, int value);
    void append(std::string name, std::string value);
    //void append(std::string name, RcppDate& date);
    //void append(std::string name, RcppDatetime& datetime);
    void append(std::string name, SEXP sexp);
    void clearProtectionStack();
    SEXP getList(void) const;

protected:
    friend class RcppResultSet;

private:
    SEXP listArg;
    int listSize, currListPosn, numProtected;
    std::vector<std::string> names;
};

#endif
