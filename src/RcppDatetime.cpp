// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppDatetime.h: Rcpp R/C++ interface class library -- Datetime type support
//
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

#include <RcppDatetime.h>

RcppDatetime::RcppDatetime(void) : m_d(0), 
								   m_parsed(false), 
								   m_us(0) { 
};

RcppDatetime::RcppDatetime(const double d) : m_d(d), 
											 m_parsed(false), 
											 m_us(0) { 
};

void RcppDatetime::parseTime() {
	// tt is the nb of seconds, ignores fractional microsec
	time_t tt = static_cast<time_t>(floor(m_d));	
	m_tm = *localtime(&tt);			// parse time type into time structure 

	// m_us is fractional (micro)secs is diff. between (fractional) m_d and m_tm
	m_us = static_cast<int>(round( (m_d - tt) * 1.0e6));	

	m_parsed = true;				// and note that we parsed the time type
};

std::ostream& operator<<(std::ostream& os, const RcppDatetime &datetime) {
	RcppDatetime dt(datetime);
	dt.parseTime();
	char buf[32], usec[16];
	strftime(buf, 31, "%Y-%m-%d %H:%M:%S", &dt.m_tm);
	snprintf(usec, 15, ".%.06d", dt.m_us);
	os << buf << usec;
	return os;
};

RcppDatetime operator+(const RcppDatetime &date,   double offset) {
	RcppDatetime tmp(date.m_d);
	tmp.m_d += offset;
	tmp.m_parsed = false;
	return tmp;
};
