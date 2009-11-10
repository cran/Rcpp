// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppDate.h: Rcpp R/C++ interface class library -- Date type support
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

#ifndef RcppDate_h
#define RcppDate_h

#include <RcppCommon.h>

class RcppDate {
private:
    void mdy2jdn(); 						// month/day/year to Julian day number.
    void jdn2mdy(); 						// Julian day number to month/day/year.
    int month, day, year;
    int jdn; 								// Julian day number

public:
    static const int Jan1970Offset;			// offset between Jan 1, 1970 and Julian Day Number
    static const int QLtoJan1970Offset;		// offset between Jan 1, 1970 and QuantLib BaseDate
    RcppDate();
    RcppDate(int Rjdn);
    RcppDate(int month_, int day_, int year_);
    int getMonth() const { return month; }
    int getDay()  const  { return day; }
    int getYear() const  { return year; }
    int getJDN()  const  { return jdn; }

    // Minimal set of date operations.
    // These operators tend to conflict with QuantLib's
    friend RcppDate operator+(const RcppDate &date, int offset);
    friend int      operator-(const RcppDate& date1, const RcppDate& date2);
    friend bool     operator<(const RcppDate &date1, const RcppDate& date2);
    friend bool     operator>(const RcppDate &date1, const RcppDate& date2);
    friend bool     operator==(const RcppDate &date1, const RcppDate& date2);
    friend bool     operator>=(const RcppDate &date1, const RcppDate& date2);
    friend bool     operator<=(const RcppDate &date1, const RcppDate& date2);

    friend std::ostream& operator<<(std::ostream& os, const RcppDate& date);
// #ifdef USING_QUANTLIB
//     // Conversions from/to a QuantLib Date.
//     RcppDate(Date dateQL);
//     operator Date() const;
// #endif
};

#endif
